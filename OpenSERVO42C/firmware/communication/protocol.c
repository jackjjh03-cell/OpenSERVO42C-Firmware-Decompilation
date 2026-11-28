#include "protocol.h"
uint16_t os_crc16(const uint8_t *p,size_t n) {
    uint16_t crc=0xffffu;
    for(size_t i=0;i<n;i++){
        crc=(uint16_t)(crc ^ ((uint16_t)p[i]<<8));
        for(unsigned b=0;b<8;b++)
            crc=(uint16_t)((uint32_t)crc<<1 ^ ((crc&0x8000u)?0x1021u:0u));
    }
    return crc;
}
static uint16_t get16(const uint8_t *p){return (uint16_t)(p[0]|((uint16_t)p[1]<<8));}
static uint32_t get32(const uint8_t *p){return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void put16(uint8_t *p,uint16_t n){p[0]=(uint8_t)n;p[1]=(uint8_t)(n>>8);}
static void put32(uint8_t *p,uint32_t n){put16(p,(uint16_t)n);put16(p+2,(uint16_t)(n>>16));}
static void encode_cfg(uint8_t *p,const os_config *v){
    put32(p,v->following_counts);put32(p+4,v->stall_error_counts);
    put32(p+8,v->stall_speed_counts_s);put16(p+12,v->stall_load_permille);
    put16(p+14,v->dwell_ms);put16(p+16,v->clear_ms);p[18]=v->policy;
}
size_t os_dispatch(os_controller *c,const uint8_t *in,size_t n,uint8_t *out){
    if(n<8u || in[0]!=0xa5u || in[1]!=0x5au || in[2]!=OS_PROTO_VERSION ||
       in[5]>OS_MAX_PAYLOAD || n!=8u+in[5] ||
       get16(in+n-2)!=os_crc16(in+2,n-4)) return 0;
    uint8_t len=1u, *p=out+6;
    p[0]=OS_OK;
    if(in[4]!=OS_SET_CONFIG && in[5]!=0u) p[0]=OS_BAD_LENGTH;
    else switch(in[4]){
    case OS_HELLO: p[1]=0; p[2]=1; p[3]=0; p[4]=0; len=5; break; /* no hardware capabilities */
    case OS_STATUS:
        p[1]=(uint8_t)c->state; p[2]=(uint8_t)c->mode;
        put32(p+3,c->faults);put32(p+7,c->observed);put32(p+11,c->warnings);
        put32(p+15,(uint32_t)c->latest.target);put32(p+19,(uint32_t)c->latest.actual);
        put32(p+23,(uint32_t)c->latest.speed_counts_s);put16(p+27,c->latest.load_permille);
        p[29]=(uint8_t)c->position_lost;len=30;break;
    case OS_GET_CONFIG: encode_cfg(p+1,&c->cfg);len=20;break;
    case OS_SET_CONFIG:
        if(in[5]!=19u){p[0]=OS_BAD_LENGTH;break;}
        {const uint8_t *q=in+6;os_config v={get32(q),get32(q+4),get32(q+8),
            get16(q+12),get16(q+14),get16(q+16),q[18]};
         if(!os_configure(c,&v))p[0]=OS_REJECTED;}
        break;
    case OS_DISABLE: os_disable(c);break;
    case OS_ACK: if(!os_acknowledge(c))p[0]=OS_REJECTED;break;
    default:p[0]=OS_UNSUPPORTED;break;
    }
    out[0]=0xa5;out[1]=0x5a;out[2]=OS_PROTO_VERSION;out[3]=in[3];
    out[4]=(uint8_t)(in[4]|0x80u);out[5]=len;
    put16(out+6+len,os_crc16(out+2,4u+len));
    return 8u+len;
}
