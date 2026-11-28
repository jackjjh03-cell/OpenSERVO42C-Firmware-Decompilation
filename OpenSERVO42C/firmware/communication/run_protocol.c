#include "run_protocol.h"
static uint16_t get16(const uint8_t *p){return (uint16_t)(p[0]|((uint16_t)p[1]<<8));}
static uint32_t get32(const uint8_t *p){return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void put32(uint8_t *p,uint32_t n){for(unsigned i=0;i<4;i++)p[i]=(uint8_t)(n>>(8u*i));}
size_t os_run_dispatch(os_run *r,const os_board_io *io,const uint8_t *in,size_t n,uint8_t *out){
    if(n<8u||in[0]!=0xa5u||in[1]!=0x5au||in[2]!=OS_PROTO_VERSION||in[5]>OS_MAX_PAYLOAD||n!=8u+in[5]||get16(in+n-2)!=os_crc16(in+2,n-4))return 0;
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
        else if(r->enable||r->gate.enabled||in[6]>OS_POSITION||!os_select_mode(c,(os_mode)in[6]))p[0]=OS_REJECTED;
        break;
    case OS_MOVE_ABS:
        if(in[5]!=4u)p[0]=OS_BAD_LENGTH;
        else if(!r->enable||!r->gate.enabled||r->drive.pending_fault||!os_start_move(c,(int32_t)get32(in+6)))p[0]=OS_REJECTED;
        break;
    case OS_GET_RUN:
        if(in[5]){p[0]=OS_BAD_LENGTH;break;}
        p[1]=(uint8_t)((r->gate.enabled?1u:0u)|(r->enable?2u:0u)|(r->adc_good?4u:0u)|(r->drive.phase_valid?8u:0u));
        p[2]=r->drive.pending_fault;put32(p+3,r->last_tick);put32(p+7,r->adc_count);put32(p+11,r->missed_ticks);len=15;break;
    default:return os_dispatch(c,in,n,out);
    }
    out[0]=0xa5;out[1]=0x5a;out[2]=OS_PROTO_VERSION;out[3]=in[3];out[4]=(uint8_t)(in[4]|0x80u);out[5]=len;
    uint16_t crc=os_crc16(out+2,4u+len);out[6u+len]=(uint8_t)crc;out[7u+len]=(uint8_t)(crc>>8);return 8u+len;
}
