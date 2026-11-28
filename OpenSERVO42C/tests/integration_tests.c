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
    puts("Controller integration tests passed");return 0;
}
