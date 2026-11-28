#include "trace.h"
void os_trace_tick(os_trace *t,const os_controller *c,uint32_t now){
    if(!t->running)return;
    if(c->state!=OS_FAULT&&!t->first&&(uint32_t)(now-t->next)>=0x80000000u)return;
    t->first=false;t->next=now+t->interval;
    int32_t d=c->demand;if(d>INT16_MAX)d=INT16_MAX;if(d<INT16_MIN)d=INT16_MIN;
    t->sample[t->count++]=(os_trace_sample){now,c->latest.actual,c->latest.target,(int16_t)d,c->latest.load_permille};
    if(c->state==OS_FAULT)t->faulted=true;
    if(t->count==t->requested||t->faulted)t->running=false;
}
size_t os_trace_dispatch(os_trace *t,const uint8_t *in,size_t n,uint8_t *out){
    if(!os_frame_valid(in,n))return 0;
    uint8_t len=1;out[6]=OS_OK;
    switch(in[4]){
    case OS_TRACE_START:
        if(in[5]!=4u){out[6]=OS_BAD_LENGTH;break;}
        {uint16_t count=os_wire_get16(in+6),interval=os_wire_get16(in+8);
        if(t->running||!count||count>OS_TRACE_CAPACITY||!interval){out[6]=OS_REJECTED;break;}
        t->faulted=false;t->count=0;t->requested=count;t->interval=interval;t->first=true;t->running=true;}
        break;
    case OS_TRACE_STOP:
        if(in[5])out[6]=OS_BAD_LENGTH;else t->running=false;break;
    case OS_TRACE_STATUS:
        if(in[5]){out[6]=OS_BAD_LENGTH;break;}
        os_wire_put16(out+7,t->count);os_wire_put16(out+9,t->requested);os_wire_put16(out+11,t->interval);out[13]=(uint8_t)((t->running?1u:0u)|(t->faulted?2u:0u));len=8;break;
    case OS_TRACE_READ:
        if(in[5]!=3u){out[6]=OS_BAD_LENGTH;break;}
        {uint16_t offset=os_wire_get16(in+6);uint8_t count=in[8];
        if(t->running||!count||count>2u||offset>=t->count||count>t->count-offset){out[6]=OS_REJECTED;break;}
        os_wire_put16(out+7,offset);out[9]=count;len=4;
        for(unsigned i=0;i<count;i++){
            const os_trace_sample *s=&t->sample[offset+i];uint8_t *p=out+6+len;
            os_wire_put32(p,s->tick);os_wire_put32(p+4,(uint32_t)s->actual);os_wire_put32(p+8,(uint32_t)s->target);
            os_wire_put16(p+12,(uint16_t)s->demand);os_wire_put16(p+14,s->load);len+=16;
        }}break;
    default:out[6]=OS_UNSUPPORTED;
    }
    return os_frame_reply(in,out,len);
}
