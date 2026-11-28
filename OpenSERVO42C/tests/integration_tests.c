#include "controller.h"
#include "protocol.h"
#include <assert.h>
#include <stdio.h>
static os_sample healthy(void){return (os_sample){.enable_input=true,.feedback_valid=true,.reference_valid=true};}
static void tick(os_controller *c,os_sample *s,unsigned n){while(n--)os_tick_1ms(c,s);}
static void setup(os_controller *c,os_sample *s,os_mode mode){
    os_init(c);assert(os_select_mode(c,mode));assert(!os_arm(c));
    c->hal_validated=true; /* test only */
    *s=healthy();tick(c,s,100);assert(os_arm(c));
}
int main(void){
    os_controller c;os_sample s;
    setup(&c,&s,OS_CNC);s.target=150;tick(&c,&s,1);assert(c.latest.target==150 && c.demand==150);
    assert(!os_start_move(&c,200));assert(!os_set_pid(&c,&c.pid_cfg));assert(!os_select_mode(&c,OS_POSITION));
    s.enable_input=false;tick(&c,&s,1);assert(c.state==OS_OFF && c.demand==0);
    setup(&c,&s,OS_POSITION);assert(os_start_move(&c,5000));assert(!os_start_move(&c,1000));
    s.target=INT32_MAX;tick(&c,&s,200);assert(c.latest.target>0 && c.latest.target<5000 && c.demand>0);
    s.feedback_valid=false;tick(&c,&s,1);assert(c.state==OS_FAULT && c.demand==0 && !c.moving);
    assert(c.faults&OS_F_ENCODER);assert(c.position_lost);
    setup(&c,&s,OS_POSITION);s.reference_valid=false;tick(&c,&s,1);assert(c.faults&OS_F_REFERENCE);
    setup(&c,&s,OS_POSITION);assert(os_start_move(&c,5000));
    for(unsigned n=0;n<10000 && c.moving;n++){s.actual=c.latest.target;tick(&c,&s,1);}
    assert(c.latest.target==5000 && !c.moving);
    os_init(&c);os_motion_config m=c.motion_cfg;m.curve=OS_TRAPEZOID;assert(os_set_motion(&c,&m));
    m.speed=0;assert(!os_set_motion(&c,&m));assert(c.motion_cfg.speed>0);
    setup(&c,&s,OS_VELOCITY);assert(!os_set_rate(&c,10001,0));assert(!os_set_rate(&c,1000,50));
    assert(os_set_rate(&c,1000,0));tick(&c,&s,1);assert(c.rate_milli==30000);
    for(unsigned n=0;n<1000;n++){s.actual=c.latest.target;tick(&c,&s,1);}
    assert(c.rate_milli==1000000&&c.command>900&&c.command<1000);
    assert(os_set_rate(&c,-1000,0));tick(&c,&s,1);assert(c.rate_milli==985000);
    for(unsigned n=0;n<150;n++){s.actual=c.latest.target;tick(&c,&s,1);}
    assert(c.rate_milli==-1000000);assert(os_set_rate(&c,0,0));
    for(unsigned n=0;n<100;n++){s.actual=c.latest.target;tick(&c,&s,1);}
    assert(!c.moving&&c.rate_milli==0);
    setup(&c,&s,OS_JOG);assert(!os_set_rate(&c,1000,0));assert(os_set_rate(&c,1000,50));
    for(unsigned n=0;n<200;n++){s.actual=c.latest.target;tick(&c,&s,1);}
    assert(!c.moving&&!c.jog_ms&&!c.requested_rate);int32_t held=c.command;
    tick(&c,&s,100);assert(c.command==held);
    assert(os_set_rate(&c,-1000,50));tick(&c,&s,1);s.enable_input=false;tick(&c,&s,1);
    assert(c.state==OS_OFF&&!c.rate_milli&&!c.requested_rate&&!c.jog_ms);
    setup(&c,&s,OS_VELOCITY);c.limits=(os_limits){-100,100,true};assert(os_set_rate(&c,1000,0));
    for(unsigned n=0;n<300;n++){s.actual=c.latest.target;tick(&c,&s,1);assert(c.command<=100&&c.state==OS_ACTIVE);}
    assert(!c.moving&&c.command>0);s.actual=101;tick(&c,&s,1);assert(c.faults&OS_F_TRAVEL);
    setup(&c,&s,OS_TORQUE);assert(!os_set_torque(&c,500,0));assert(!os_set_torque(&c,2048,100));
    assert(os_set_torque(&c,500,3));tick(&c,&s,1);assert(c.demand==500&&c.torque_ms==2);
    s.actual=9000;s.target=-9000;tick(&c,&s,1);assert(c.demand==500&&!c.follow_ms&&!c.stall_ms);
    tick(&c,&s,1);assert(!c.demand&&c.torque_cutoff==OS_TORQUE_TIMEOUT);
    assert(os_set_torque(&c,-500,100));s.speed_counts_s=-10001;tick(&c,&s,1);
    assert(!c.demand&&c.torque_cutoff==OS_TORQUE_SPEED);s.speed_counts_s=0;tick(&c,&s,1);assert(!c.demand);
    setup(&c,&s,OS_TORQUE);c.limits=(os_limits){-100,100,true};assert(os_set_torque(&c,500,100));
    s.actual=100;tick(&c,&s,1);assert(!c.demand&&c.torque_cutoff==OS_TORQUE_LIMIT);
    assert(os_set_torque(&c,-500,100));tick(&c,&s,1);assert(c.demand==-500);
    s.actual=101;tick(&c,&s,1);assert(c.faults&OS_F_TRAVEL);assert(!c.demand&&!c.torque_ms);
    setup(&c,&s,OS_TORQUE);assert(os_set_torque(&c,500,100));s.reference_valid=false;tick(&c,&s,1);
    assert(c.faults&OS_F_REFERENCE);assert(!c.demand&&!c.requested_torque);
    puts("Controller integration tests passed");return 0;
}
