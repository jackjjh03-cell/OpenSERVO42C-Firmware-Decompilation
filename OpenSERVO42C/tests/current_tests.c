#include "current.h"
#include <assert.h>
#include <stdio.h>
static bool inactive(os_pwm_compare p){return p.a0==1200 && p.a1==1200 && p.b0==1200 && p.b1==1200;}
int main(void){
    os_offset_cal c;os_offsets o;os_offset_begin(&c);
    assert(!os_offset_finish(&c,&o));
    assert(os_offset_add(&c,0,4095));assert(os_offset_add(&c,4095,0));
    for(unsigned i=0;i<16;i++)assert(os_offset_add(&c,2048,2000));
    assert(os_offset_finish(&c,&o)&&o.slot2==2048&&o.slot1==2000);
    os_pair f;assert(os_current_feedback(&o,2000,2100,&f)&&f.a==48&&f.b==100);
    assert(!os_current_feedback(&o,4096,0,&f));
    os_offset_begin(&c);for(unsigned i=0;i<18;i++)assert(os_offset_add(&c,1800,2000));
    assert(!os_offset_finish(&c,&o));
    os_pwm_compare p;os_pair zero={0,0},target={100,-100};
    assert(os_current_step(zero,zero,0,16384,2048,2000,1080,&p)&&inactive(p));
    assert(os_current_step(zero,target,0,16384,2048,2000,1080,&p));
    assert(p.a0<1200&&p.a1==1200&&p.b0<1200&&p.b1==1200);
    assert(os_current_step(zero,target,16384,0,2048,2000,1080,&p));
    assert(p.a0<1200&&p.a1==1200&&p.b0==1200&&p.b1<1200);
    assert(!os_current_step(zero,target,0,16384,2048,744,1080,&p)&&inactive(p));
    assert(!os_current_step(zero,target,0,16384,2048,2000,1201,&p)&&inactive(p));
    for(unsigned scale=745;scale<=4095;scale+=17){
        for(int a=-8000;a<=8000;a+=500){
            target=(os_pair){a,-a};
            assert(os_current_step((os_pair){4095,-4095},target,11585,11585,65535,(uint16_t)scale,1080,&p));
            assert(p.a0>=120&&p.a1>=120&&p.b0>=120&&p.b1>=120);
            assert(p.a0==1200||p.a1==1200);assert(p.b0==1200||p.b1==1200);
        }
    }
    puts("Current regulation tests passed");return 0;
}
