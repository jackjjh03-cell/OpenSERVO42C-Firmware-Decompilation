#include "run_protocol.h"
size_t os_run_dispatch(os_run *r,const os_board_io *io,const uint8_t *in,size_t n,uint8_t *out){
    if(!os_frame_valid(in,n))return 0;
    uint8_t len=1u,*p=out+6;p[0]=OS_OK;
    os_controller *c=&r->drive.control;
    switch(in[4]){
    case OS_ARM:
        if(in[5])p[0]=OS_BAD_LENGTH;
        else if(!os_run_arm(r,io))p[0]=OS_REJECTED;
        break;
    case OS_DISABLE:
        if(in[5])p[0]=OS_BAD_LENGTH;else os_run_disable(r,io);
        break;
    case OS_ACK:
        if(in[5])p[0]=OS_BAD_LENGTH;
        else if(r->enable||!r->adc_good||r->drive.pending_fault||!os_acknowledge(c))p[0]=OS_REJECTED;
        break;
    case OS_SELECT_MODE:
        if(in[5]!=1u)p[0]=OS_BAD_LENGTH;
        else if(r->teaching||r->enable||r->gate.enabled||in[6]>OS_TORQUE||!os_select_mode(c,(os_mode)in[6]))p[0]=OS_REJECTED;
        break;
    case OS_MOVE_ABS:
        if(in[5]!=4u)p[0]=OS_BAD_LENGTH;
        else if(!r->enable||!r->gate.enabled||r->drive.pending_fault||!os_start_move(c,(int32_t)os_wire_get32(in+6)))p[0]=OS_REJECTED;
        break;
    case OS_GET_RUN:
        if(in[5]){p[0]=OS_BAD_LENGTH;break;}
        p[1]=(uint8_t)((r->gate.enabled?1u:0u)|(r->enable?2u:0u)|(r->adc_good?4u:0u)|(r->drive.phase_valid?8u:0u)|(r->teaching?16u:0u)|(c->latest.reference_valid&&!c->position_lost?32u:0u));
        p[2]=r->drive.pending_fault;os_wire_put32(p+3,r->last_tick);os_wire_put32(p+7,r->adc_count);os_wire_put32(p+11,r->missed_ticks);len=15;break;
    case OS_GET_LIMITS:
        if(in[5]){p[0]=OS_BAD_LENGTH;break;}
        os_wire_put32(p+1,(uint32_t)c->limits.minimum);os_wire_put32(p+5,(uint32_t)c->limits.maximum);p[9]=(uint8_t)c->limits.enabled;len=10;break;
    case OS_SET_LIMITS:
        if(in[5]!=9u||in[14]>1u){p[0]=OS_BAD_LENGTH;break;}
        {os_limits v={(int32_t)os_wire_get32(in+6),(int32_t)os_wire_get32(in+10),in[14]!=0u};
         if(r->teaching||r->enable||r->gate.enabled||!os_set_limits(c,&v))p[0]=OS_REJECTED;}
        break;
    case OS_CONFIRM_REFERENCE:
        if(in[5]!=4u)p[0]=OS_BAD_LENGTH;
        else if(!os_run_confirm_reference(r,(int32_t)os_wire_get32(in+6)))p[0]=OS_REJECTED;
        break;
    case OS_SET_LIVE_PID:
        if(in[5]!=18u){p[0]=OS_BAD_LENGTH;break;}
        {os_pid_config v={os_wire_get32(in+6),os_wire_get32(in+10),os_wire_get32(in+14),os_wire_get32(in+18),os_wire_get16(in+22)};
         if(!r->enable||!r->gate.enabled||!r->adc_good||r->drive.pending_fault||!os_set_live_pid(c,&v))p[0]=OS_REJECTED;}
        break;
    case OS_GET_LIVE_PID:
        if(in[5]){p[0]=OS_BAD_LENGTH;break;}
        {const os_pid_config *v=c->live_pid?&c->live_pid_cfg:&c->pid_cfg;
         p[1]=(uint8_t)c->live_pid;os_wire_put32(p+2,v->kp);os_wire_put32(p+6,v->ki);os_wire_put32(p+10,v->kd);os_wire_put32(p+14,v->deadband);
         p[18]=(uint8_t)v->output_limit;p[19]=(uint8_t)(v->output_limit>>8);len=20;}
        break;
    case OS_TEACH_BEGIN:
        if(in[5])p[0]=OS_BAD_LENGTH;
        else if(!os_run_teach_begin(r,io))p[0]=OS_REJECTED;
        break;
    case OS_TEACH_CAPTURE:
        if(in[5]!=1u||in[6]>1u){p[0]=OS_BAD_LENGTH;break;}
        {int32_t position;if(!os_run_teach_capture(r,in[6]!=0u,&position))p[0]=OS_REJECTED;
         else{os_wire_put32(p+1,(uint32_t)position);len=5;}}
        break;
    case OS_TEACH_APPLY:
        if(in[5]!=8u)p[0]=OS_BAD_LENGTH;
        else if(!os_run_teach_apply(r,os_wire_get32(in+6),os_wire_get32(in+10)))p[0]=OS_REJECTED;
        break;
    case OS_TEACH_CANCEL:
        if(in[5])p[0]=OS_BAD_LENGTH;else os_run_teach_cancel(r);
        break;
    case OS_SET_RATE:
        if(in[5]!=6u)p[0]=OS_BAD_LENGTH;
        else if(!r->enable||!r->gate.enabled||!r->adc_good||r->drive.pending_fault||
            !os_set_rate(c,(int32_t)os_wire_get32(in+6),os_wire_get16(in+10)))p[0]=OS_REJECTED;
        break;
    case OS_GET_RATE:
        if(in[5]){p[0]=OS_BAD_LENGTH;break;}
        os_wire_put32(p+1,(uint32_t)c->requested_rate);os_wire_put32(p+5,(uint32_t)c->rate_milli);
        os_wire_put16(p+9,c->jog_ms);len=11;break;
    case OS_SET_TORQUE:
        if(in[5]!=6u)p[0]=OS_BAD_LENGTH;
        else{
            int32_t demand=(int32_t)os_wire_get32(in+6),limit=r->drive.cfg.phase_limit;
            if(!r->enable||!r->gate.enabled||!r->adc_good||r->drive.pending_fault||demand>limit||demand< -limit||
                !os_set_torque(c,demand,os_wire_get16(in+10)))p[0]=OS_REJECTED;
        }
        break;
    case OS_GET_TORQUE:
        if(in[5]){p[0]=OS_BAD_LENGTH;break;}
        os_wire_put32(p+1,(uint32_t)c->requested_torque);os_wire_put32(p+5,(uint32_t)(c->mode==OS_TORQUE?c->demand:0));
        os_wire_put16(p+9,c->torque_ms);p[11]=c->torque_cutoff;len=12;break;
    default:return os_dispatch(c,in,n,out);
    }
    return os_frame_reply(in,out,len);
}
