#include "motion.h"
#include "pid.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
static void trajectory(os_motion_config c,int32_t start,int32_t end){
    os_motion m;
    assert(os_motion_plan(&m,start,end,&c));
    assert(os_motion_position(&m,0)==start);
    int32_t last=start;
    for(uint32_t t=1;t<=m.total_ms;t++){
        int32_t x=os_motion_tick(&m);
        assert(end>=start?(x>=last && x<=end):(x<=last && x>=end));
        last=x;
    }
    assert(last==end);assert(os_motion_tick(&m)==end);
}
/* Independent original 64-bit planning formulas, including invalid spans. */
static uint64_t div_up(uint64_t n,uint64_t d){return n/d+(n%d!=0u);}
static uint32_t ref_ramp(uint32_t v,uint32_t a){return (uint32_t)div_up((uint64_t)v*1000u,a);}
static bool ref_smooth(uint32_t d,uint32_t t,const os_motion_config *c){
    uint32_t a=c->accel<c->decel?c->accel:c->decel;
    uint64_t t2=(uint64_t)t*t;
    return t>=div_up((uint64_t)d*2000u,c->speed)&&t2>=div_up((uint64_t)d*6000000u,a)&&
        t2*t>=div_up((uint64_t)d*60000000000ULL,c->jerk);
}
static void compare_plan(os_motion_config c,uint32_t d){
    os_motion m;bool ok=os_motion_plan(&m,-1000000,-1000000+(int32_t)d,&c);
    if(c.curve==OS_S_CURVE){
        assert(ok==ref_smooth(d,600000,&c));
        if(ok){assert(ref_smooth(d,m.total_ms,&c));assert(!ref_smooth(d,m.total_ms-1u,&c));}
    }else{
        uint32_t lo=1,hi=c.speed;
        while(lo<hi){uint32_t v=lo+(hi-lo+1u)/2u;
            if((uint64_t)v*(ref_ramp(v,c.accel)+ref_ramp(v,c.decel))<=(uint64_t)d*2000u)lo=v;else hi=v-1u;
        }
        uint32_t up=ref_ramp(lo,c.accel),down=ref_ramp(lo,c.decel);
        uint64_t span=div_up((uint64_t)d*2000u,lo),ramps=(uint64_t)up+down;
        uint32_t cruise=span>ramps?(uint32_t)div_up(span-ramps,2u):0u;
        assert(ok==(ramps+cruise<=600000u));
        if(ok)assert(m.up_ms==up&&m.down_ms==down&&m.cruise_ms==cruise&&m.total_ms==ramps+cruise);
    }
}
static uint32_t rng(uint32_t *seed){*seed=*seed*1664525u+1013904223u;return *seed;}
int main(void){
    for(unsigned curve=0;curve<2;curve++){
        os_motion_config c={10000,30000,15000,60000,(uint8_t)curve};
        trajectory(c,0,100000);trajectory(c,12345,-45678);trajectory(c,0,1);trajectory(c,0,0);
        trajectory(c,INT32_MAX-1000,INT32_MAX);
        os_motion m;assert(!os_motion_plan(&m,INT32_MIN,INT32_MAX,&c));
        c.speed=0;assert(!os_motion_plan(&m,0,10,&c));
    }
    os_pid p;os_pid_reset(&p);os_pid_config c={4096,512,4096,2,100};
    assert(os_pid_step(&p,&c,10000,0,false)==100);
    for(int i=0;i<1000;i++)assert(os_pid_step(&p,&c,10000,0,false)==100);
    assert(p.integral_q12==0);
    assert(os_pid_step(&p,&c,0,0,true)==0);
    assert(os_pid_step(&p,&c,0,10,false)<0);
    assert(os_pid_step(&p,&c,INT32_MAX,INT32_MIN,false)==100);
    c.output_limit=0;assert(os_pid_step(&p,&c,1,0,false)==0);
    for(unsigned i=0;i<1000;i++){
        os_motion_config v={1000u+(unsigned)rand()%100000u,1000u+(unsigned)rand()%100000u,
            1000u+(unsigned)rand()%100000u,1000u+(unsigned)rand()%100000u,(uint8_t)(i%2u)};
        os_motion m;int32_t end=1+rand()%1000000;
        if(os_motion_plan(&m,0,end,&v))trajectory(v,0,end);
    }
    uint32_t seed=1234567u;
    for(unsigned i=0;i<10000u;i++){
        uint32_t speed=1u+rng(&seed)%1000000u,accel=1u+rng(&seed)%100000000u;
        uint32_t decel=1u+rng(&seed)%100000000u,jerk=1u+rng(&seed)%1000000000u;
        uint32_t d=1u+rng(&seed)%1000000u;
        for(unsigned curve=0;curve<2;curve++)compare_plan((os_motion_config){speed,accel,decel,jerk,(uint8_t)curve},d);
    }
    for(unsigned curve=0;curve<2;curve++)for(unsigned i=0;i<16;i++){
        os_motion_config v={(i&1u)?1u:1000000u,(i&2u)?1u:100000000u,(i&4u)?1u:100000000u,(i&8u)?1u:1000000000u,(uint8_t)curve};
        compare_plan(v,1);compare_plan(v,1000000);
    }
    puts("Motion and PID tests passed");return 0;
}
