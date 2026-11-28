#include "current.h"
#include <assert.h>
#include <stdio.h>
static bool inactive(os_pwm_compare p){return p.a0==1200 && p.a1==1200 && p.b0==1200 && p.b1==1200;}
/* Independent wider arithmetic reference for the fixed-point regulator. */
static int64_t round_ref(int64_t x,int64_t den){return x<0?-((-x+den/2)/den):(x+den/2)/den;}
static int64_t bound(int64_t v,int64_t lim){return v>lim?lim:v< -lim?-lim:v;}
static uint16_t compare_ref(int64_t v,uint32_t scale,uint16_t cap){
    int64_t factor=(0x12c000u+scale/2u)/scale;
    int64_t duty=round_ref((v<0?-v:v)*factor,1024);
    return (uint16_t)(1200-(duty>cap?cap:duty));
}
static os_pwm_compare reference(os_pair f,os_pair t,int16_t s,int16_t c,uint16_t gain,uint16_t scale,uint16_t cap){
    int64_t d=round_ref((int64_t)c*f.a+(int64_t)s*f.b,16384);
    int64_t q=round_ref((int64_t)c*f.b-(int64_t)s*f.a,16384);
    int64_t lim=round_ref((int64_t)scale*972,1024);
    d=bound(round_ref((t.a-d)*gain,2048),lim);q=bound(round_ref((t.b-q)*gain,2048),lim);
    int64_t a=round_ref(c*d-s*q,16384),b=round_ref(s*d+c*q,16384);
    uint16_t x=compare_ref(a,scale,cap),y=compare_ref(b,scale,cap);
    return (os_pwm_compare){a<0?1200:x,a<0?x:1200,b<0?y:1200,b<0?1200:y};
}
static uint32_t rng(uint32_t *seed){*seed=*seed*1664525u+1013904223u;return *seed;}
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
    uint32_t seed=321765u;
    for(unsigned i=0;i<20000u;i++){
        os_pair fb={(int32_t)(rng(&seed)%8191u)-4095,(int32_t)(rng(&seed)%8191u)-4095};
        os_pair t={(int32_t)(rng(&seed)%16001u)-8000,(int32_t)(rng(&seed)%16001u)-8000};
        int16_t sin=(int16_t)((int32_t)(rng(&seed)%32769u)-16384),cos=(int16_t)((int32_t)(rng(&seed)%32769u)-16384);
        uint16_t gain=(uint16_t)rng(&seed),scale=(uint16_t)(745u+rng(&seed)%3351u),cap=(uint16_t)(rng(&seed)%1201u);
        os_pwm_compare expected=reference(fb,t,sin,cos,gain,scale,cap);
        assert(os_current_step(fb,t,sin,cos,gain,scale,cap,&p));
        assert(p.a0==expected.a0&&p.a1==expected.a1&&p.b0==expected.b0&&p.b1==expected.b1);
        assert(p.a0>=1200u-cap&&p.a1>=1200u-cap&&p.b0>=1200u-cap&&p.b1>=1200u-cap);
        assert((p.a0==1200||p.a1==1200)&&(p.b0==1200||p.b1==1200));
    }
    puts("Current regulation tests passed");return 0;
}
