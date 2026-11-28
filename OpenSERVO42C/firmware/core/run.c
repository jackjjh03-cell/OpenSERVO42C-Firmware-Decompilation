#include "run.h"
static uint32_t mag(int32_t x){return x<0?0u-(uint32_t)x:(uint32_t)x;}
static void close_gate(os_run *r,const os_board_io *io){if(r->gate.enabled)os_gate_close(&r->gate,io);}
static void trip(os_run *r,const os_board_io *io,uint8_t bits){
    r->drive.pending_fault|=bits;close_gate(r,io);
}
bool os_run_init(os_run *r,const os_board_io *io,const os_drive_config *cfg,const os_offsets *offsets){
    *r=(os_run){0};os_gate_close(&r->gate,io);return os_drive_init(&r->drive,cfg,offsets);
}
void os_run_disable(os_run *r,const os_board_io *io){
    os_gate_close(&r->gate,io);os_drive_disable(&r->drive);
}
void os_run_enable(os_run *r,const os_board_io *io,bool enable){
    r->enable=enable;
    if(!enable){
        if(r->gate.enabled||r->drive.control.state==OS_ACTIVE)r->drive.inhibit_requested=true;
        close_gate(r,io);
    }
}
void os_run_enable_edge(os_run *r,const os_board_io *io,bool enable){
    /* A latched edge may represent a disable/re-enable pair. Do not resume
     * motion until a fresh explicit arm, even if the input is high now. */
    os_run_enable(r,io,false);r->enable=enable;
}
void os_run_tick(os_run *r,const os_board_io *io,uint32_t now,uint16_t angle,bool valid,uint16_t pulse,bool enable,bool reference,bool travel){
    if(r->tick_seen && now==r->last_tick)return;
    bool active=r->drive.control.state==OS_ACTIVE;
    uint32_t delta=now-r->last_tick;
    if(r->tick_seen&&delta>1u){
        uint32_t missed=delta-1u;
        r->missed_ticks=UINT32_MAX-r->missed_ticks<missed?UINT32_MAX:r->missed_ticks+missed;
        if(active||r->teaching||r->reference_confirmed||r->drive.control.latest.reference_valid)trip(r,io,OS_F_TIMING);
        r->drive.control.healthy_ms=0;
    }
    if(r->tick_seen&&r->adc_count==r->last_count){
        r->adc_good=false;
        if(active||r->teaching||r->reference_confirmed||r->drive.control.latest.reference_valid)trip(r,io,OS_F_TIMING);
    }
    r->last_count=r->adc_count;r->last_tick=now;r->tick_seen=true;
    os_run_enable(r,io,enable);
    os_drive_position(&r->drive,angle,valid,pulse,enable,(reference||r->reference_confirmed)&&!r->drive.control.position_lost,travel);
    if(r->drive.control.position_lost){r->reference_confirmed=false;r->captured=0;r->drive.control.latest.reference_valid=false;}
    /* Recovery/arming require fresh healthy electrical feedback, even when off. */
    if(!r->adc_good)r->drive.control.healthy_ms=0;
    if(r->drive.control.state!=OS_ACTIVE)close_gate(r,io);
}
void os_run_adc(os_run *r,const os_board_io *io,bool enable,bool angle_valid,uint16_t angle,bool valid,uint16_t scale,uint16_t slot2,uint16_t slot1){
    r->adc_count++;r->adc_seen=true;os_run_enable(r,io,enable);
    os_pair fb;
    r->adc_good=r->drive.configured&&valid&&scale>=745u&&scale<=4095u&&os_current_feedback(&r->drive.offsets,slot2,slot1,&fb)&&mag(fb.a)<=r->drive.cfg.trip_limit&&mag(fb.b)<=r->drive.cfg.trip_limit;
    if(!r->adc_good){trip(r,io,OS_F_ELECTRICAL);return;}
    if(!angle_valid&&(r->gate.enabled||r->teaching||r->reference_confirmed)){trip(r,io,OS_F_ENCODER);return;}
    if(r->drive.control.state!=OS_ACTIVE||!r->gate.enabled)return;
    os_pwm_compare p;
    if(!os_drive_current(&r->drive,enable,angle_valid,angle,valid,scale,slot2,slot1,&p)){close_gate(r,io);return;}
    if(!os_gate_update(&r->gate,io,enable,&p))trip(r,io,OS_F_ELECTRICAL);
}
bool os_run_confirm_reference(os_run *r,int32_t position){
    const os_limits *l=&r->drive.control.limits;
    if(r->teaching||r->enable||r->gate.enabled||!r->adc_good||!r->drive.phase_valid||
       (l->enabled&&(position<l->minimum||position>l->maximum))||
       !os_drive_confirm_reference(&r->drive,position,r->drive.encoder.previous,r->drive.previous_pulse))return false;
    r->reference_confirmed=true;return true;
}
bool os_run_arm(os_run *r,const os_board_io *io){
    if(r->teaching||!r->enable||!r->tick_seen||!r->adc_seen||!r->adc_good||r->gate.enabled||!os_drive_arm(&r->drive))return false;
    if(!os_gate_open(&r->gate,io,true)){trip(r,io,OS_F_ELECTRICAL);os_drive_disable(&r->drive);return false;}
    return true;
}

static bool teach_ready(const os_run *r){
    const os_controller *c=&r->drive.control;
    return r->tick_seen&&r->adc_good&&r->drive.phase_valid&&!r->gate.enabled&&
        c->state==OS_OFF&&!c->faults&&!r->drive.pending_fault&&!c->position_lost&&
        c->latest.feedback_valid&&c->latest.reference_valid;
}
bool os_run_teach_begin(os_run *r,const os_board_io *io){
    if(r->teaching||!teach_ready(r))return false;
    os_run_disable(r,io);r->captured=0;r->teaching=true;return true;
}
bool os_run_teach_capture(os_run *r,bool maximum,int32_t *position){
    if(!r->teaching||!teach_ready(r)||!position||
       mag(r->drive.control.latest.speed_counts_s)>r->drive.control.cfg.stall_speed_counts_s)return false;
    *position=r->drive.control.latest.actual;
    if(maximum){r->taught_max=*position;r->captured|=2u;}
    else{r->taught_min=*position;r->captured|=1u;}
    return true;
}
bool os_run_teach_apply(os_run *r,uint32_t lower_margin,uint32_t upper_margin){
    if(!r->teaching||r->captured!=3u||!teach_ready(r))return false;
    int64_t lo=(int64_t)r->taught_min+lower_margin,hi=(int64_t)r->taught_max-upper_margin;
    if(lo>=hi||lo<INT32_MIN||hi>INT32_MAX)return false;
    /* Only this locked, output-disabled setup session can change limits with EN high. */
    r->drive.control.limits=(os_limits){(int32_t)lo,(int32_t)hi,true};
    r->drive.control.healthy_ms=0;r->teaching=false;r->captured=0;return true;
}
void os_run_teach_cancel(os_run *r){r->teaching=false;r->captured=0;}
