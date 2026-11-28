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
    if(!enable){close_gate(r,io);r->drive.inhibit_requested=true;}
}
void os_run_tick(os_run *r,const os_board_io *io,uint32_t now,uint16_t angle,bool valid,uint16_t pulse,bool enable,bool reference,bool travel){
    if(r->tick_seen && now==r->last_tick)return;
    bool active=r->drive.control.state==OS_ACTIVE;
    uint32_t delta=now-r->last_tick;
    if(r->tick_seen&&delta>1u){
        uint32_t missed=delta-1u;
        r->missed_ticks=UINT32_MAX-r->missed_ticks<missed?UINT32_MAX:r->missed_ticks+missed;
        if(active)trip(r,io,OS_F_TIMING);
        r->drive.control.healthy_ms=0;
    }
    if(r->tick_seen&&r->adc_count==r->last_count){
        r->adc_good=false;
        if(active)trip(r,io,OS_F_TIMING);
    }
    r->last_count=r->adc_count;r->last_tick=now;r->tick_seen=true;
    os_run_enable(r,io,enable);
    os_drive_position(&r->drive,angle,valid,pulse,enable,reference,travel);
    /* Recovery/arming require fresh healthy electrical feedback, even when off. */
    if(!r->adc_good)r->drive.control.healthy_ms=0;
    if(r->drive.control.state!=OS_ACTIVE)close_gate(r,io);
}
void os_run_adc(os_run *r,const os_board_io *io,bool enable,bool angle_valid,uint16_t angle,bool valid,uint16_t scale,uint16_t slot2,uint16_t slot1){
    r->adc_count++;r->adc_seen=true;os_run_enable(r,io,enable);
    os_pair fb;
    r->adc_good=r->drive.configured&&valid&&scale>=745u&&scale<=4095u&&os_current_feedback(&r->drive.offsets,slot2,slot1,&fb)&&mag(fb.a)<=r->drive.cfg.trip_limit&&mag(fb.b)<=r->drive.cfg.trip_limit;
    if(!r->adc_good){trip(r,io,OS_F_ELECTRICAL);return;}
    if(r->drive.control.state!=OS_ACTIVE||!r->gate.enabled)return;
    os_pwm_compare p;
    if(!os_drive_current(&r->drive,enable,angle_valid,angle,valid,scale,slot2,slot1,&p)){close_gate(r,io);return;}
    if(!os_gate_update(&r->gate,io,enable,&p))trip(r,io,OS_F_ELECTRICAL);
}
bool os_run_arm(os_run *r,const os_board_io *io){
    if(!r->enable||!r->tick_seen||!r->adc_seen||!r->adc_good||r->gate.enabled||!os_drive_arm(&r->drive))return false;
    if(!os_gate_open(&r->gate,io,true)){trip(r,io,OS_F_ELECTRICAL);os_drive_disable(&r->drive);return false;}
    return true;
}
