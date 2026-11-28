#include "calibration_sweep.h"
#include "phase.h"
static int32_t raw_delta(uint16_t a,uint16_t b){return (int32_t)(((uint32_t)b+8192u-a)&16383u)-8192;}
void os_cal_stop(os_cal_sweep *s,os_run *r,const os_board_io *io,bool completed){
    if(!(s->flags&OS_CAL_ACTIVE))return;
    os_run_disable(r,io);s->flags=completed?(OS_CAL_DONE|OS_CAL_READY):OS_CAL_ABORTED;
    r->drive.inhibit_requested=false;r->adc_good=false;r->tick_seen=false;
    r->drive.control.healthy_ms=0;r->drive.phase_valid=false;
    os_encoder_reset(&r->drive.encoder);
}
void os_cal_adc(os_cal_sweep *s,os_run *r,const os_board_io *io,bool enable,bool raw_valid,bool valid,uint16_t scale,uint16_t slot2,uint16_t slot1){
    os_pair fb;os_pwm_compare p;
    r->adc_count++;r->adc_seen=true;os_run_enable(r,io,enable);
    r->adc_good=os_drive_feedback(&r->drive,valid,scale,slot2,slot1,&fb);
    if(!r->adc_good)r->drive.pending_fault|=OS_F_ELECTRICAL;
    if(!raw_valid)r->drive.pending_fault|=OS_F_ENCODER;
    if(!enable||!r->gate.enabled||r->drive.pending_fault||r->drive.inhibit_requested){os_cal_stop(s,r,io,false);return;}
    if(!os_current_step(fb,(os_pair){s->current,0},os_sine_q14(s->phase),os_sine_q14((uint16_t)(s->phase+256u)),r->drive.cfg.current_gain_q11,scale,r->drive.cfg.compare_limit,&p)||
       !os_gate_update(&r->gate,io,true,&p)){
        r->drive.pending_fault|=OS_F_ELECTRICAL;
        os_cal_stop(s,r,io,false);
    }
}
void os_cal_tick(os_cal_sweep *s,os_run *r,const os_board_io *io,uint32_t now,bool fresh,bool raw_valid,uint16_t raw){
    if(!(s->flags&OS_CAL_ACTIVE))return;
    if(!fresh||now-s->last_tick!=1u)r->drive.pending_fault|=OS_F_TIMING;
    int32_t d=raw_delta(s->previous,raw);
    if(!raw_valid||d*4>(int32_t)r->drive.cfg.max_encoder_delta||d*4<-(int32_t)r->drive.cfg.max_encoder_delta)r->drive.pending_fault|=OS_F_ENCODER;
    s->last_tick=now;s->previous=raw;
    if(!r->enable||!r->gate.enabled||!r->adc_good||r->drive.pending_fault||r->drive.inhibit_requested||
       (uint32_t)(now-s->deadline)<0x80000000u){os_cal_stop(s,r,io,false);return;}
    if(s->flags&OS_CAL_READY)return;
    if(s->wait){s->wait--;return;}
    if(s->remaining){
        s->phase=(uint16_t)(s->phase+(s->index<=200u?128u:65408u));
        s->remaining--;s->wait=49;return;
    }
    if(!s->samples)s->base=raw;
    s->sum+=raw_delta(s->base,raw);
    if(++s->samples==32u){
        s->raw=(uint16_t)(((int32_t)s->base+s->sum/32)&16383);
        s->flags|=OS_CAL_READY;
        if(s->index==400u)os_cal_stop(s,r,io,true);
    }
}
size_t os_cal_dispatch(os_cal_sweep *s,os_run *r,const os_board_io *io,uint32_t now,bool raw_valid,uint16_t raw,const uint8_t *in,size_t n,uint8_t *out){
    if(!os_frame_valid(in,n))return 0;
    out[6]=OS_OK;uint8_t len=1;
    os_controller *c=&r->drive.control;
    switch(in[4]){
    case OS_CAL_START:
        if(in[5]!=2u){out[6]=OS_BAD_LENGTH;break;}
        {uint16_t current=os_wire_get16(in+6);
        /* An uncoupled motor only. Never bypass enabled mechanical limits,
         * electrical/timing faults, or a live motion/teaching session. */
        if((s->flags&OS_CAL_ACTIVE)||r->gate.enabled||r->teaching||!r->enable||!r->adc_good||!raw_valid||!r->tick_seen||
           !r->drive.configured||!c->hal_validated||c->limits.enabled||c->state==OS_ACTIVE||
           (c->faults&~(uint32_t)(OS_F_ENCODER|OS_F_REFERENCE))||r->drive.pending_fault||r->drive.inhibit_requested||!current||current>r->drive.cfg.phase_limit){out[6]=OS_REJECTED;break;}
        r->reference_confirmed=false;c->latest.reference_valid=false;c->position_lost=true;r->drive.phase_valid=false;
        os_run_disable(r,io);
        *s=(os_cal_sweep){.last_tick=now,.deadline=now+5000u,.previous=raw,.current=current,.wait=1200,.flags=OS_CAL_ACTIVE};
        if(!os_gate_open(&r->gate,io,true)){s->flags=OS_CAL_ABORTED;r->drive.pending_fault|=OS_F_ELECTRICAL;out[6]=OS_REJECTED;}}
        break;
    case OS_CAL_STEP:
        if(in[5]){out[6]=OS_BAD_LENGTH;break;}
        if(s->flags!=(OS_CAL_ACTIVE|OS_CAL_READY)||s->index>=400u||!r->gate.enabled||!r->enable||r->drive.pending_fault){out[6]=OS_REJECTED;break;}
        s->index++;s->flags=OS_CAL_ACTIVE;s->remaining=(uint8_t)(r->drive.cfg.geometry==25u?2u:4u);
        s->samples=0;s->sum=0;s->deadline=now+5000u;break;
    case OS_CAL_STATUS:
        if(in[5]){out[6]=OS_BAD_LENGTH;break;}
        os_wire_put16(out+7,s->index);os_wire_put16(out+9,s->raw);out[11]=s->flags;len=6;break;
    case OS_CAL_STOP:
        if(in[5])out[6]=OS_BAD_LENGTH;else os_cal_stop(s,r,io,false);break;
    default:out[6]=OS_UNSUPPORTED;
    }
    return os_frame_reply(in,out,len);
}
