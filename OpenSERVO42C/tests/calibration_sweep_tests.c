#include "calibration_sweep.h"
#include <assert.h>
#include <stdio.h>
static const uint32_t addresses[]={0x40000c30u,0x40000d30u,0x40000c3cu,0x40000c40u,0x40000d3cu,0x40000d40u};
typedef struct {uint32_t regs[6];} mock;
static unsigned idx(uint32_t a){for(unsigned i=0;i<6;i++)if(addresses[i]==a)return i;assert(0);return 0;}
static uint32_t rd(void *ctx,uint32_t a){return ((mock*)ctx)->regs[idx(a)];}
static void wr(void *ctx,uint32_t a,uint32_t v){((mock*)ctx)->regs[idx(a)]=v;}
static uint8_t cmd(os_cal_sweep *s,os_run *r,const os_board_io *io,uint32_t tick,uint8_t op,uint16_t current){
    uint8_t n=op==OS_CAL_START?2:0,in[OS_FRAME_MAX]={0xa5,0x5a,1,0,op,n},out[OS_FRAME_MAX];
    if(n)os_wire_put16(in+6,current);
    os_wire_put16(in+6+n,os_crc16(in+2,4u+n));
    assert(os_cal_dispatch(s,r,io,tick,true,16380,in,8u+n,out)>=9u);
    assert(os_frame_valid(out,8u+out[5]));
    if(op==OS_CAL_STATUS){assert(os_wire_get16(out+7)==s->index&&os_wire_get16(out+9)==s->raw&&out[11]==s->flags);}
    return out[6];
}
static void init(os_cal_sweep *s,os_run *r,const os_board_io *io,uint8_t geometry){
    os_drive_config cfg={16,1000,500,2048,300,geometry,false,600};os_offsets off={2048,2048};
    assert(os_run_init(r,io,&cfg,&off));*s=(os_cal_sweep){0};r->enable=true;r->adc_good=true;r->tick_seen=true;
    r->drive.control.state=OS_FAULT;r->drive.control.faults=OS_F_ENCODER;r->drive.control.position_lost=true;
}
static void settle(os_cal_sweep *s,os_run *r,const os_board_io *io,uint32_t *tick,uint16_t raw){
    for(unsigned i=0;i<1500u&&(s->flags&OS_CAL_ACTIVE)&&!(s->flags&OS_CAL_READY);i++){
        os_cal_adc(s,r,io,true,true,true,2000,2048,2048);
        ++*tick;os_cal_tick(s,r,io,*tick,true,true,raw);
    }
    assert(s->flags&OS_CAL_READY);
}
int main(void){mock m={0};os_board_io io={&m,rd,wr,0,0};os_run r;os_cal_sweep s;
    for(uint8_t geom=25;geom<=50;geom+=25){
        init(&s,&r,&io,geom);uint32_t t=UINT32_MAX-50u;
        assert(cmd(&s,&r,&io,t,OS_CAL_START,501)==OS_REJECTED);
        r.drive.control.limits.enabled=true;assert(cmd(&s,&r,&io,t,OS_CAL_START,200)==OS_REJECTED);r.drive.control.limits.enabled=false;
        r.drive.control.faults|=OS_F_ELECTRICAL;assert(cmd(&s,&r,&io,t,OS_CAL_START,200)==OS_REJECTED);r.drive.control.faults=OS_F_ENCODER;
        assert(cmd(&s,&r,&io,t,OS_CAL_START,200)==OS_OK&&r.gate.enabled);
        assert(cmd(&s,&r,&io,t,OS_CAL_STEP,0)==OS_REJECTED);
        settle(&s,&r,&io,&t,16380);assert(s.index==0&&s.raw==16380);
        for(unsigned i=1;i<=400;i++){
            assert(cmd(&s,&r,&io,t,OS_CAL_STEP,0)==OS_OK);
            unsigned knot=i<=200?i:400-i;
            uint16_t raw=(uint16_t)((16380u+knot*16384u/200u)&16383u);
            settle(&s,&r,&io,&t,raw);assert(s.index==i&&s.raw==raw);
            assert((s.phase&1023u)==(knot*(geom==25?256u:512u)&1023u));
        }
        assert(s.flags==(OS_CAL_DONE|OS_CAL_READY)&&!r.gate.enabled&&r.drive.control.position_lost);
        assert(cmd(&s,&r,&io,t,OS_CAL_STATUS,0)==OS_OK);
    }
    init(&s,&r,&io,25);assert(cmd(&s,&r,&io,0,OS_CAL_START,200)==OS_OK);
    os_cal_adc(&s,&r,&io,true,true,true,2000,1400,2048);assert(s.flags==OS_CAL_ABORTED&&!r.gate.enabled&&(r.drive.pending_fault&OS_F_ELECTRICAL));
    init(&s,&r,&io,25);assert(cmd(&s,&r,&io,0,OS_CAL_START,200)==OS_OK);
    os_run_enable_edge(&r,&io,true);os_cal_adc(&s,&r,&io,true,true,true,2000,2048,2048);assert(s.flags==OS_CAL_ABORTED&&!r.gate.enabled);
    init(&s,&r,&io,25);assert(cmd(&s,&r,&io,0,OS_CAL_START,200)==OS_OK);
    os_cal_tick(&s,&r,&io,1,false,true,16380);assert(s.flags==OS_CAL_ABORTED&&(r.drive.pending_fault&OS_F_TIMING));
    init(&s,&r,&io,25);assert(cmd(&s,&r,&io,0,OS_CAL_START,200)==OS_OK);
    os_cal_tick(&s,&r,&io,1,true,false,16380);assert(s.flags==OS_CAL_ABORTED&&(r.drive.pending_fault&OS_F_ENCODER));
    init(&s,&r,&io,25);assert(cmd(&s,&r,&io,0,OS_CAL_START,200)==OS_OK);
    uint32_t t=0;settle(&s,&r,&io,&t,16380);while(t<5000){++t;os_cal_tick(&s,&r,&io,t,true,true,16380);}
    assert(s.flags==OS_CAL_ABORTED&&!r.gate.enabled);
    init(&s,&r,&io,25);assert(cmd(&s,&r,&io,0,OS_CAL_START,200)==OS_OK);
    assert(cmd(&s,&r,&io,1,OS_CAL_STOP,0)==OS_OK&&!r.gate.enabled&&s.flags==OS_CAL_ABORTED);
    puts("calibration sweep tests passed");return 0;
}
