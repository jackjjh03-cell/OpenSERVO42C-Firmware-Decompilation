#include "run.h"
#include <assert.h>
#include <stdio.h>
static const os_drive_config cfg={16,1000,500,2048,300,25,false,600};
static const os_offsets offsets={2048,2048};
static const uint32_t addr[]={0x40000c30u,0x40000d30u,0x40000c3cu,0x40000c40u,0x40000d3cu,0x40000d40u};
typedef struct {uint32_t v[6];} mock;
static unsigned idx(uint32_t a){for(unsigned i=0;i<6;i++)if(addr[i]==a)return i;assert(0);return 0;}
static uint32_t read(void *p,uint32_t a){return ((mock *)p)->v[idx(a)];}
static void write(void *p,uint32_t a,uint32_t v){((mock *)p)->v[idx(a)]=v;}
static void adc(os_run *r,const os_board_io *io,bool en){os_run_adc(r,io,en,true,0,true,2000,2048,2048);}
static void tick(os_run *r,const os_board_io *io,uint32_t now,bool en){adc(r,io,en);os_run_tick(r,io,now,0,true,0,en,false,false);}
static void start(os_run *r,const os_board_io *io,uint32_t base){
    assert(os_run_init(r,io,&cfg,&offsets));assert(!os_run_arm(r,io));
    for(unsigned i=0;i<100;i++)tick(r,io,base+i,true);
    assert(os_run_arm(r,io)&&r->gate.enabled);
}
static void closed(os_run *r,mock *m){assert(!r->gate.enabled&&!(m->v[0]&0x1000u)&&!(m->v[1]&0x1000u));for(unsigned i=2;i<6;i++)assert((m->v[i]&65535u)==1200);}
int main(void){mock m={0};os_board_io io={&m,read,write,0,0};os_run r;
    start(&r,&io,0);os_run_tick(&r,&io,100,0,true,0,true,false,false);closed(&r,&m);assert(r.drive.control.faults&OS_F_TIMING);
    start(&r,&io,0);tick(&r,&io,102,true);closed(&r,&m);assert(r.missed_ticks==2&&r.drive.control.faults&OS_F_TIMING);
    start(&r,&io,UINT32_MAX-99u);tick(&r,&io,0,true);assert(r.gate.enabled&&!r.missed_ticks);
    os_run_enable(&r,&io,false);closed(&r,&m);os_run_enable(&r,&io,true);adc(&r,&io,true);closed(&r,&m);tick(&r,&io,1,true);assert(r.drive.control.state==OS_OFF);
    start(&r,&io,0);os_run_adc(&r,&io,true,true,0,true,2000,1400,2048);closed(&r,&m);tick(&r,&io,100,true);assert(r.drive.control.faults&OS_F_ELECTRICAL);
    start(&r,&io,0);os_run_adc(&r,&io,true,false,0,true,2000,2048,2048);closed(&r,&m);tick(&r,&io,100,true);assert(r.drive.control.faults&OS_F_ENCODER);
    start(&r,&io,0);os_run_disable(&r,&io);closed(&r,&m);assert(r.drive.control.state==OS_OFF);
    assert(os_run_init(&r,&io,&cfg,&offsets));for(unsigned i=0;i<200;i++)os_run_tick(&r,&io,i,0,true,0,true,false,false);assert(!os_run_arm(&r,&io));closed(&r,&m);
    puts("scheduler tests passed");return 0;
}
