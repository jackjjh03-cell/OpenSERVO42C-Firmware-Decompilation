#include "bringup.h"
size_t os_bench_dispatch(os_controller *c,const os_bench *b,const uint8_t *in,size_t n,uint8_t *out){
    if(n!=8u||in[4]!=OS_GET_BENCH)return os_dispatch(c,in,n,out);
    if(!os_frame_valid(in,n))return 0;
    uint8_t *p=out+6;p[0]=OS_OK;
    os_wire_put32(p+1,b->ms);os_wire_put32(p+5,b->core_hz);os_wire_put32(p+9,b->adc_samples);os_wire_put32(p+13,b->adc_errors);
    os_wire_put32(p+17,b->encoder_errors);os_wire_put32(p+21,b->missed_ticks);os_wire_put32(p+25,b->uart_dropped);
    os_wire_put16(p+29,b->scale);os_wire_put16(p+31,b->slot1);os_wire_put16(p+33,b->slot2);os_wire_put16(p+35,b->raw_angle);
    os_wire_put16(p+37,b->cal_angle);os_wire_put16(p+39,b->pulse);os_wire_put16(p+41,b->offset1);os_wire_put16(p+43,b->offset2);p[45]=b->board_status;p[46]=b->encoder_status;p[47]=b->flags;
    return os_frame_reply(in,out,48);
}
