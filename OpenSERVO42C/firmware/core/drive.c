#include "drive.h"
#include "phase.h"
static uint32_t mag(int32_t n){return n<0?0u-(uint32_t)n:(uint32_t)n;}
bool os_drive_config_valid(const os_drive_config *v){
    return (v->geometry==25u||v->geometry==50u) && v->microsteps>0u && v->microsteps<=256u &&
        v->max_encoder_delta>0u && v->max_encoder_delta<32768u && v->phase_limit>0u && v->phase_limit<=2047u &&
        v->trip_limit>v->phase_limit && v->trip_limit<=4095u && v->current_gain_q11>0u && v->compare_limit>0u && v->compare_limit<=1080u;
}
bool os_drive_init(os_drive *d,const os_drive_config *cfg,const os_offsets *offsets){
    *d=(os_drive){0};os_init(&d->control);
    os_pair check;
    if(!os_drive_config_valid(cfg)||!os_current_feedback(offsets,offsets->slot2,offsets->slot1,&check))return false;
    d->cfg=*cfg;d->offsets=*offsets;d->configured=true;
    /* Target is responsible for peripheral readiness and commissioning. */
    d->control.hal_validated=true;
    return true;
}
static bool pulse_update(os_drive *d,uint16_t pulse){
    uint16_t raw=(uint16_t)(pulse-d->previous_pulse);
    d->previous_pulse=pulse;
    if(raw==32768u)return false; /* ambiguous half-turn of hardware counter */
    int32_t delta=raw<32768u?(int32_t)raw:(int32_t)raw-65536;
    if(d->cfg.reverse)delta=-delta;
    uint32_t denom=(uint32_t)d->cfg.geometry*8u*d->cfg.microsteps;
    int64_t numerator=(int64_t)delta*65536+d->remainder;
    /* |delta| <= 32767 and |remainder| < denom <= 102400.
     * The magnitude fits uint32_t even when the signed sum exceeds INT32_MAX.
     * Divide in 32 bits, retaining C's truncation-toward-zero convention. */
    bool negative=numerator<0;
    uint32_t magnitude=(uint32_t)(negative?-numerator:numerator);
    int32_t q=(int32_t)(magnitude/denom),rem=(int32_t)(magnitude%denom);
    int64_t target=(int64_t)d->pulse_target+(negative?-q:q);
    if(target<INT32_MIN||target>INT32_MAX)return false;
    d->pulse_target=(int32_t)target;d->remainder=negative?-rem:rem;return true;
}
void os_drive_position(os_drive *d,uint16_t angle,bool angle_valid,uint16_t pulse,bool enable,bool reference,bool travel){
    int64_t old=d->encoder.position;
    bool valid=d->configured && angle_valid && os_encoder_track(&d->encoder,angle,d->cfg.max_encoder_delta);
    if(d->encoder.position<INT32_MIN||d->encoder.position>INT32_MAX)valid=false;
    /* Reacquire a lost encoder angle only with output disabled and a latched
     * fault. The coordinate remains invalid until operator confirmation. */
    if(!valid&&d->configured&&angle_valid&&!enable&&d->control.state==OS_FAULT&&d->control.position_lost){
        os_encoder_reset(&d->encoder);old=0;
        valid=os_encoder_track(&d->encoder,angle,d->cfg.max_encoder_delta);
        d->control.healthy_ms=0;
    }
    d->phase_valid=valid;
    int32_t actual=valid?(int32_t)d->encoder.position:d->control.latest.actual;
    uint8_t critical=d->pending_fault;d->pending_fault=0;
    if(d->inhibit_requested){enable=false;d->inhibit_requested=false;}
    if(d->control.state==OS_ACTIVE && (d->control.mode==OS_CNC||d->control.mode==OS_OPEN_LOOP) && enable){
        if(!pulse_update(d,pulse))critical|=OS_F_TIMING;
    }else{
        d->previous_pulse=pulse;d->pulse_target=actual;d->remainder=0;
    }
    os_sample sample={.target=d->pulse_target,.actual=actual,.speed_counts_s=valid?(int32_t)(d->encoder.position-old)*1000:0,
        .load_permille=d->load,.critical=critical,.reference_valid=reference,.travel_violation=travel,
        .enable_input=enable,.feedback_valid=valid};
    os_tick_1ms(&d->control,&sample);
    if(d->control.state==OS_ACTIVE&&d->control.mode==OS_OPEN_LOOP)d->control.demand=d->cfg.phase_limit;
    if(d->control.state!=OS_ACTIVE)d->fast_phase_valid=false;
}
bool os_drive_arm(os_drive *d){
    if(!d->configured||!d->phase_valid||d->pending_fault||d->inhibit_requested||!os_arm(&d->control))return false;
    /* Align the commanded field with the measured rotor angle at arm. */
    d->open_offset=(uint16_t)((uint32_t)d->encoder.previous-(uint32_t)d->pulse_target);
    return true;
}
bool os_drive_confirm_reference(os_drive *d,int32_t position,uint16_t angle,uint16_t pulse){
    os_controller *c=&d->control;
    if(!d->configured||c->state!=OS_OFF||c->faults||c->latest.enable_input||!c->latest.feedback_valid||
       c->observed||c->healthy_ms<c->cfg.clear_ms||c->latest.speed_counts_s!=0||d->pending_fault||d->inhibit_requested)return false;
    if(!d->encoder.initialized||angle!=d->encoder.previous)return false;
    d->encoder.position=position;d->pulse_target=position;d->previous_pulse=pulse;d->remainder=0;
    c->command=position;c->latest.target=position;c->latest.actual=position;c->position_lost=false;
    d->fast_phase_valid=false;os_pid_reset(&c->pid);return true;
}
void os_drive_disable(os_drive *d){d->fast_phase_valid=false;os_disable(&d->control);}
bool os_drive_feedback(os_drive *d,bool adc_valid,uint16_t scale,uint16_t slot2,uint16_t slot1,os_pair *feedback){
    if(!d->configured||!adc_valid||scale<745u||scale>4095u||!os_current_feedback(&d->offsets,slot2,slot1,feedback))return false;
    uint32_t a=mag(feedback->a),b=mag(feedback->b),peak=a>b?a:b;
    if(peak>d->cfg.trip_limit)return false;
    d->load=(uint16_t)(peak>=d->cfg.phase_limit?1000u:peak*1000u/d->cfg.phase_limit);
    return true;
}
bool os_drive_current_checked(os_drive *d,bool enable,bool angle_valid,uint16_t angle,uint16_t scale,os_pair feedback,os_pwm_compare *p){
    os_pwm_inactive(p);
    if(!enable){d->inhibit_requested=true;d->fast_phase_valid=false;return false;}
    if(!d->configured || d->pending_fault || d->inhibit_requested || d->control.state!=OS_ACTIVE || !d->phase_valid)return false;
    if(!angle_valid){d->pending_fault|=OS_F_ENCODER;return false;}
    uint16_t wrap=(uint16_t)(angle-d->previous_angle);
    uint32_t jump=wrap<=32768u?wrap:65536u-wrap;
    if(d->fast_phase_valid && jump>d->cfg.max_encoder_delta){d->pending_fault|=OS_F_ENCODER;return false;}
    int32_t demand=d->control.demand;
    /* Bound requested vector magnitude independently of PID settings. */
    int32_t limit=d->cfg.phase_limit;
    if(demand>limit)demand=limit;else if(demand< -limit)demand=-limit;
    os_pair target={0,demand};
    if(d->control.mode==OS_OPEN_LOOP){
        /* Open-loop position: command the field from STEP/DIR, not the
         * measured rotor position. Encoder and current protection stay on. */
        uint16_t commanded=(uint16_t)((uint32_t)d->pulse_target+d->open_offset);
        d->phase=os_electrical_phase(commanded,commanded,false,0,d->cfg.geometry);
        target=(os_pair){limit,0};
    }else d->phase=os_electrical_phase(angle,d->previous_angle,d->fast_phase_valid,demand,d->cfg.geometry);
    d->previous_angle=angle;d->fast_phase_valid=true;
    if(!os_current_step(feedback,target,os_sine_q14(d->phase),os_sine_q14((uint16_t)(d->phase+256u)),
            d->cfg.current_gain_q11,scale,d->cfg.compare_limit,p)){
        d->pending_fault|=OS_F_ELECTRICAL;return false;
    }
    return true;
}

bool os_drive_current(os_drive *d,bool enable,bool angle_valid,uint16_t angle,bool adc_valid,
                      uint16_t scale,uint16_t slot2,uint16_t slot1,os_pwm_compare *p){
    /* Preserve the standalone API's inhibit and fault precedence. */
    if(!enable||!d->configured||d->pending_fault||d->inhibit_requested||d->control.state!=OS_ACTIVE||!d->phase_valid||!angle_valid)
        return os_drive_current_checked(d,enable,angle_valid,angle,scale,(os_pair){0},p);
    os_pair fb;
    if(!os_drive_feedback(d,adc_valid,scale,slot2,slot1,&fb)){
        os_pwm_inactive(p);d->pending_fault|=OS_F_ELECTRICAL;return false;
    }
    return os_drive_current_checked(d,enable,angle_valid,angle,scale,fb,p);
}
