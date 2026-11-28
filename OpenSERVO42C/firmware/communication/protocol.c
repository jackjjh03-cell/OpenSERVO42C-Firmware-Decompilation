#include "protocol.h"
__attribute__((noinline)) uint16_t os_crc16(const uint8_t *p,size_t n) {
    uint16_t crc=0xffffu;
    for(size_t i=0;i<n;i++){
        crc=(uint16_t)(crc ^ ((uint16_t)p[i]<<8));
        for(unsigned b=0;b<8;b++)
            crc=(uint16_t)((uint32_t)crc<<1 ^ ((crc&0x8000u)?0x1021u:0u));
    }
    return crc;
}
__attribute__((noinline)) uint16_t os_wire_get16(const uint8_t *p){return (uint16_t)(p[0]|((uint16_t)p[1]<<8));}
__attribute__((noinline)) uint32_t os_wire_get32(const uint8_t *p){return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
__attribute__((noinline)) void os_wire_put16(uint8_t *p,uint16_t n){p[0]=(uint8_t)n;p[1]=(uint8_t)(n>>8);}
__attribute__((noinline)) void os_wire_put32(uint8_t *p,uint32_t n){os_wire_put16(p,(uint16_t)n);os_wire_put16(p+2,(uint16_t)(n>>16));}
#if !defined(__BYTE_ORDER__) || __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error Configuration codec requires a little-endian target
#endif
_Static_assert(offsetof(os_config,following_counts)==0 && offsetof(os_config,stall_error_counts)==4 &&
    offsetof(os_config,stall_speed_counts_s)==8 && offsetof(os_config,stall_load_permille)==12 &&
    offsetof(os_config,dwell_ms)==14 && offsetof(os_config,clear_ms)==16 && offsetof(os_config,policy)==18,"config wire layout");
_Static_assert(offsetof(os_pid_config,kp)==0 && offsetof(os_pid_config,ki)==4 && offsetof(os_pid_config,kd)==8 &&
    offsetof(os_pid_config,deadband)==12 && offsetof(os_pid_config,output_limit)==16,"PID wire layout");
_Static_assert(offsetof(os_motion_config,speed)==0 && offsetof(os_motion_config,accel)==4 && offsetof(os_motion_config,decel)==8 &&
    offsetof(os_motion_config,jerk)==12 && offsetof(os_motion_config,curve)==16,"motion wire layout");
__attribute__((noinline)) void os_wire_copy(void *dst,const void *src,size_t n){
    uint8_t *d=dst;const uint8_t *s=src;while(n--)*d++=*s++;
}
size_t os_dispatch(os_controller *c,const uint8_t *in,size_t n,uint8_t *out){
    if(!os_frame_valid(in,n))return 0;
    uint8_t len=1u, *p=out+6;
    p[0]=OS_OK;
    if(in[4]!=OS_SET_CONFIG && in[4]!=OS_SET_PID && in[4]!=OS_SET_MOTION && in[5]!=0u) p[0]=OS_BAD_LENGTH;
    else switch(in[4]){
    case OS_HELLO: p[1]=0; p[2]=1; p[3]=1; p[4]=0; len=5; break; /* no hardware capabilities */
    case OS_STATUS:
        p[1]=(uint8_t)c->state; p[2]=(uint8_t)c->mode;
        os_wire_put32(p+3,c->faults);os_wire_put32(p+7,c->observed);os_wire_put32(p+11,c->warnings);
        os_wire_put32(p+15,(uint32_t)c->latest.target);os_wire_put32(p+19,(uint32_t)c->latest.actual);
        os_wire_put32(p+23,(uint32_t)c->latest.speed_counts_s);os_wire_put16(p+27,c->latest.load_permille);
        p[29]=(uint8_t)c->position_lost;len=30;break;
    case OS_GET_CONFIG: os_wire_copy(p+1,&c->cfg,OS_CONFIG_WIRE_BYTES);len=20;break;
    case OS_SET_CONFIG:
        if(in[5]!=19u){p[0]=OS_BAD_LENGTH;break;}
        {os_config v;os_wire_copy(&v,in+6,OS_CONFIG_WIRE_BYTES);
         if(!os_configure(c,&v))p[0]=OS_REJECTED;}
        break;
    case OS_DISABLE: os_disable(c);break;
    case OS_ACK: if(!os_acknowledge(c))p[0]=OS_REJECTED;break;
    case OS_GET_PID: os_wire_copy(p+1,&c->pid_cfg,OS_PID_WIRE_BYTES);len=19;break;
    case OS_SET_PID:
        if(in[5]!=18u){p[0]=OS_BAD_LENGTH;break;}
        {os_pid_config v;os_wire_copy(&v,in+6,OS_PID_WIRE_BYTES);
         if(!os_set_pid(c,&v))p[0]=OS_REJECTED;}
        break;
    case OS_GET_MOTION: os_wire_copy(p+1,&c->motion_cfg,OS_MOTION_WIRE_BYTES);len=18;break;
    case OS_SET_MOTION:
        if(in[5]!=17u){p[0]=OS_BAD_LENGTH;break;}
        {os_motion_config v;os_wire_copy(&v,in+6,OS_MOTION_WIRE_BYTES);
         if(!os_set_motion(c,&v))p[0]=OS_REJECTED;}
        break;
    case OS_GET_CONTROL:
        os_wire_put32(p+1,(uint32_t)c->demand);os_wire_put32(p+5,c->motion.elapsed_ms);os_wire_put32(p+9,c->motion.total_ms);
        p[13]=(uint8_t)c->moving;len=14;break;
    default:p[0]=OS_UNSUPPORTED;break;
    }
    return os_frame_reply(in,out,len);
}
__attribute__((noinline)) bool os_frame_valid(const uint8_t *in,size_t n){
    return n>=8u&&in[0]==0xa5u&&in[1]==0x5au&&in[2]==OS_PROTO_VERSION&&
        in[5]<=OS_MAX_PAYLOAD&&n==8u+in[5]&&os_wire_get16(in+n-2)==os_crc16(in+2,n-4);
}
__attribute__((noinline)) size_t os_frame_reply(const uint8_t *in,uint8_t *out,uint8_t len){
    out[0]=0xa5;out[1]=0x5a;out[2]=OS_PROTO_VERSION;out[3]=in[3];
    out[4]=(uint8_t)(in[4]|0x80u);out[5]=len;
    os_wire_put16(out+6+len,os_crc16(out+2,4u+len));return 8u+len;
}
