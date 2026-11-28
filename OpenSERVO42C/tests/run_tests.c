#include "run_protocol.h"
#include "timing.h"
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
static uint8_t command(os_run *r,const os_board_io *io,uint8_t op,const uint8_t *payload,uint8_t size){
    uint8_t in[OS_FRAME_MAX]={0xa5,0x5a,1,23,op,size},out[OS_FRAME_MAX];
    for(unsigned i=0;i<size;i++)in[6+i]=payload[i];
    uint16_t crc=os_crc16(in+2,4u+size);in[6u+size]=(uint8_t)crc;in[7u+size]=(uint8_t)(crc>>8);
    assert(os_run_dispatch(r,io,in,8u+size,out)>=9u&&out[3]==23&&out[4]==(uint8_t)(op|0x80u));
    return out[6];
}
int main(void){mock m={0};os_board_io io={&m,read,write,0,0};os_run r;
    start(&r,&io,0);os_run_tick(&r,&io,100,0,true,0,true,false,false);closed(&r,&m);assert(r.drive.control.faults&OS_F_TIMING);
    start(&r,&io,0);tick(&r,&io,102,true);closed(&r,&m);assert(r.missed_ticks==2&&r.drive.control.faults&OS_F_TIMING);
    start(&r,&io,UINT32_MAX-99u);tick(&r,&io,0,true);assert(r.gate.enabled&&!r.missed_ticks);
    os_run_enable(&r,&io,false);closed(&r,&m);os_run_enable(&r,&io,true);adc(&r,&io,true);closed(&r,&m);tick(&r,&io,1,true);assert(r.drive.control.state==OS_OFF);
    start(&r,&io,0);os_run_adc(&r,&io,true,true,0,true,2000,1400,2048);closed(&r,&m);tick(&r,&io,100,true);assert(r.drive.control.faults&OS_F_ELECTRICAL);
    start(&r,&io,0);os_run_adc(&r,&io,true,false,0,true,2000,2048,2048);closed(&r,&m);tick(&r,&io,100,true);assert(r.drive.control.faults&OS_F_ENCODER);
    start(&r,&io,0);os_run_disable(&r,&io);closed(&r,&m);assert(r.drive.control.state==OS_OFF);
    assert(os_run_init(&r,&io,&cfg,&offsets));for(unsigned i=0;i<200;i++)os_run_tick(&r,&io,i,0,true,0,true,false,false);assert(!os_run_arm(&r,&io));closed(&r,&m);
    assert(os_run_init(&r,&io,&cfg,&offsets));
    uint8_t position=OS_POSITION,move[]={40,0,0,0};
    assert(command(&r,&io,OS_ARM,0,0)==OS_REJECTED);
    assert(command(&r,&io,OS_SELECT_MODE,&position,1)==OS_OK);
    for(unsigned i=0;i<100;i++){adc(&r,&io,true);os_run_tick(&r,&io,i,0,true,0,true,true,false);}
    assert(command(&r,&io,OS_ARM,0,0)==OS_OK&&r.gate.enabled);
    assert(command(&r,&io,OS_MOVE_ABS,move,4)==OS_OK&&r.drive.control.moving);
    assert(command(&r,&io,OS_SELECT_MODE,&position,1)==OS_REJECTED);
    assert(command(&r,&io,OS_DISABLE,0,0)==OS_OK);closed(&r,&m);assert(!r.drive.control.moving);
    assert(command(&r,&io,OS_GET_RUN,0,0)==OS_OK);
    assert(os_run_init(&r,&io,&cfg,&offsets));
    uint8_t limits[]={156,255,255,255,100,0,0,0,1}; /* -100..100 */
    assert(command(&r,&io,OS_SET_LIMITS,limits,9)==OS_OK);
    assert(command(&r,&io,OS_SELECT_MODE,&position,1)==OS_OK);
    for(unsigned i=0;i<100;i++){adc(&r,&io,true);os_run_tick(&r,&io,i,0,true,0,true,true,false);}
    assert(command(&r,&io,OS_ARM,0,0)==OS_OK);
    assert(command(&r,&io,OS_SET_LIMITS,limits,9)==OS_REJECTED);
    uint8_t beyond[]={101,0,0,0};assert(command(&r,&io,OS_MOVE_ABS,beyond,4)==OS_REJECTED);
    uint8_t gains[]={0,8,0,0,0,0,0,0,0,0,0,0,2,0,0,0,244,1};
    assert(command(&r,&io,OS_SET_LIVE_PID,gains,18)==OS_OK&&r.drive.control.live_pid);
    assert(r.drive.control.pid_cfg.kp==4096&&r.drive.control.live_pid_cfg.kp==2048);
    assert(command(&r,&io,OS_GET_LIVE_PID,0,0)==OS_OK);
    adc(&r,&io,true);os_run_tick(&r,&io,100,101,true,0,true,true,false);
    assert(r.drive.control.faults&OS_F_TRAVEL);closed(&r,&m);assert(!r.drive.control.live_pid);
    assert(command(&r,&io,OS_CONFIRM_REFERENCE,move,4)==OS_REJECTED);
    assert(os_run_init(&r,&io,&cfg,&offsets));
    for(unsigned i=0;i<100;i++)tick(&r,&io,i,false);
    assert(command(&r,&io,OS_CONFIRM_REFERENCE,move,4)==OS_OK&&r.drive.encoder.position==40&&r.reference_confirmed);
    assert(command(&r,&io,OS_SELECT_MODE,&position,1)==OS_OK);
    tick(&r,&io,100,true);assert(r.drive.control.latest.reference_valid);
    assert(command(&r,&io,OS_ARM,0,0)==OS_OK);
    adc(&r,&io,true);os_run_tick(&r,&io,101,10000,true,0,true,false,false);
    assert(r.drive.control.faults&OS_F_ENCODER);assert(!r.reference_confirmed);closed(&r,&m);
    for(unsigned i=102;i<202;i++){adc(&r,&io,false);os_run_tick(&r,&io,i,10000,true,0,false,false,false);}
    assert(command(&r,&io,OS_ACK,0,0)==OS_OK);
    assert(r.drive.control.position_lost&&!r.reference_confirmed);
    assert(command(&r,&io,OS_CONFIRM_REFERENCE,move,4)==OS_OK);
    assert(r.reference_confirmed&&!r.drive.control.position_lost);
    assert(os_run_init(&r,&io,&cfg,&offsets));
    for(unsigned i=0;i<100;i++)tick(&r,&io,i,false);
    assert(!os_run_teach_begin(&r,&io));
    assert(os_run_confirm_reference(&r,0));tick(&r,&io,100,true);
    assert(command(&r,&io,OS_TEACH_BEGIN,0,0)==OS_OK);
    assert(command(&r,&io,OS_ARM,0,0)==OS_REJECTED);
    uint8_t low=0,high=1,margin[]={10,0,0,0,20,0,0,0};
    adc(&r,&io,true);os_run_tick(&r,&io,101,65000,true,0,true,false,false);
    assert(command(&r,&io,OS_TEACH_CAPTURE,&low,1)==OS_REJECTED);
    adc(&r,&io,true);os_run_tick(&r,&io,102,65000,true,0,true,false,false);
    assert(command(&r,&io,OS_TEACH_CAPTURE,&low,1)==OS_OK&&r.taught_min==-536);
    assert(command(&r,&io,OS_TEACH_APPLY,margin,8)==OS_REJECTED);
    adc(&r,&io,true);os_run_tick(&r,&io,103,400,true,0,true,false,false);
    adc(&r,&io,true);os_run_tick(&r,&io,104,400,true,0,true,false,false);
    assert(command(&r,&io,OS_TEACH_CAPTURE,&high,1)==OS_OK&&r.taught_max==400);
    assert(command(&r,&io,OS_TEACH_APPLY,margin,8)==OS_OK);
    assert(!r.teaching&&r.drive.control.limits.minimum==-526&&r.drive.control.limits.maximum==380);
    closed(&r,&m);assert(command(&r,&io,OS_ARM,0,0)==OS_REJECTED);
    assert(command(&r,&io,OS_TEACH_BEGIN,0,0)==OS_OK);
    tick(&r,&io,107,true);assert(r.drive.control.faults&OS_F_TIMING);
    assert(command(&r,&io,OS_TEACH_CAPTURE,&low,1)==OS_REJECTED);
    assert(command(&r,&io,OS_TEACH_CANCEL,0,0)==OS_OK);closed(&r,&m);
    assert(os_run_init(&r,&io,&cfg,&offsets));
    for(unsigned i=0;i<100;i++)tick(&r,&io,i,false);
    assert(os_run_confirm_reference(&r,0));tick(&r,&io,100,false);
    assert(r.reference_confirmed&&r.drive.control.latest.reference_valid);
    tick(&r,&io,102,false);
    assert(r.drive.control.faults&OS_F_TIMING);
    assert(!r.reference_confirmed&&!r.drive.control.latest.reference_valid&&r.drive.control.position_lost);closed(&r,&m);
    assert(os_run_init(&r,&io,&cfg,&offsets));
    for(unsigned i=0;i<100;i++)tick(&r,&io,i,false);
    adc(&r,&io,false); /* disabled fast IRQ between tick and reference command */
    assert(!r.drive.inhibit_requested&&os_run_confirm_reference(&r,0));
    tick(&r,&io,100,false);
    os_run_adc(&r,&io,false,false,0,true,2000,2048,2048);
    tick(&r,&io,101,false);
    assert(r.drive.control.faults&OS_F_ENCODER);assert(!r.reference_confirmed);closed(&r,&m);
    start(&r,&io,0);
    os_run_enable_edge(&r,&io,true); /* latched edge, input already back high */
    closed(&r,&m);assert(r.drive.inhibit_requested);
    adc(&r,&io,true);closed(&r,&m);assert(!os_run_arm(&r,&io));
    tick(&r,&io,100,true);assert(r.drive.control.state==OS_OFF);
    tick(&r,&io,101,true);assert(!r.gate.enabled);
    assert(os_run_arm(&r,&io));
    os_timing_stat stats={0};
    assert(!os_timing_record(&stats,48000,2000,1000,false)&&stats.peak==1000);
    assert(!os_timing_record(&stats,48000,100,47900,true)&&stats.peak==1000);
    assert(os_timing_record(&stats,48000,2000,1000,true)&&stats.peak==49000&&stats.overruns==1);
    assert(!os_timing_record(&stats,48000,100,47900,false)&&stats.overruns==1);
    stats.overruns=UINT32_MAX;assert(os_timing_record(&stats,48000,1000,1000,true)&&stats.overruns==UINT32_MAX);
    puts("scheduler tests passed");return 0;
}
