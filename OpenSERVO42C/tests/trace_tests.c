#include "trace.h"
#include <assert.h>
#include <stdio.h>
static uint8_t out[OS_FRAME_MAX];
static uint8_t cmd(os_trace *t,uint8_t op,const uint8_t *p,uint8_t n){
    uint8_t in[OS_FRAME_MAX]={0xa5,0x5a,1,1,op,n};
    for(unsigned i=0;i<n;i++)in[6+i]=p[i];
    os_wire_put16(in+6+n,os_crc16(in+2,4u+n));
    assert(os_trace_dispatch(t,in,8u+n,out)>=9u&&os_frame_valid(out,8u+out[5]));return out[6];
}
int main(void){os_trace t={0};os_controller c={0};c.state=OS_ACTIVE;c.latest.actual=-10;c.latest.target=20;c.demand=-40;c.latest.load_permille=300;
    uint8_t start[]={3,0,2,0},read[]={0,0,2};
    assert(cmd(&t,OS_TRACE_START,start,4)==OS_OK);assert(cmd(&t,OS_TRACE_START,start,4)==OS_REJECTED);
    assert(cmd(&t,OS_TRACE_READ,read,3)==OS_REJECTED);
    os_trace_tick(&t,&c,UINT32_MAX-1u);os_trace_tick(&t,&c,UINT32_MAX);assert(t.count==1);
    os_trace_tick(&t,&c,0);os_trace_tick(&t,&c,2);assert(t.count==3&&!t.running);
    assert(cmd(&t,OS_TRACE_STATUS,0,0)==OS_OK&&os_wire_get16(out+7)==3&&out[13]==0);
    assert(cmd(&t,OS_TRACE_READ,read,3)==OS_OK&&out[5]==36&&os_wire_get32(out+10)==UINT32_MAX-1u);
    assert((int32_t)os_wire_get32(out+14)==-10&&(int32_t)os_wire_get32(out+18)==20&&(int16_t)os_wire_get16(out+22)==-40);
    assert(cmd(&t,OS_TRACE_START,start,4)==OS_OK);c.state=OS_FAULT;os_trace_tick(&t,&c,10);assert(!t.running&&t.count==1&&t.faulted);
    assert(cmd(&t,OS_TRACE_READ,read,3)==OS_REJECTED);assert(cmd(&t,OS_TRACE_STOP,0,0)==OS_OK);
    start[0]=161;assert(cmd(&t,OS_TRACE_START,start,4)==OS_REJECTED);puts("trace tests passed");return 0;}
