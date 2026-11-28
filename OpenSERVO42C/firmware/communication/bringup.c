#include "bringup.h"
static void put16(uint8_t *p,uint16_t v){p[0]=(uint8_t)v;p[1]=(uint8_t)(v>>8);}
static void put32(uint8_t *p,uint32_t v){put16(p,(uint16_t)v);put16(p+2,(uint16_t)(v>>16));}
size_t os_bench_dispatch(os_controller *c,const os_bench *b,const uint8_t *in,size_t n,uint8_t *out){
    if(n!=8u||in[4]!=OS_GET_BENCH)return os_dispatch(c,in,n,out);
    if(in[0]!=0xa5u||in[1]!=0x5au||in[2]!=OS_PROTO_VERSION||in[5]!=0u||
       (uint16_t)(in[6]|((uint16_t)in[7]<<8))!=os_crc16(in+2,4))return 0;
    out[0]=0xa5;out[1]=0x5a;out[2]=OS_PROTO_VERSION;out[3]=in[3];out[4]=0x8c;out[5]=48;
    uint8_t *p=out+6;p[0]=OS_OK;
    put32(p+1,b->ms);put32(p+5,b->core_hz);put32(p+9,b->adc_samples);put32(p+13,b->adc_errors);
    put32(p+17,b->encoder_errors);put32(p+21,b->missed_ticks);put32(p+25,b->uart_dropped);
    put16(p+29,b->scale);put16(p+31,b->slot1);put16(p+33,b->slot2);put16(p+35,b->raw_angle);
    put16(p+37,b->cal_angle);put16(p+39,b->pulse);put16(p+41,b->offset1);put16(p+43,b->offset2);p[45]=b->board_status;p[46]=b->encoder_status;p[47]=b->flags;
    put16(p+48,os_crc16(out+2,52));return 56;
}
