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
    puts("Motion and PID tests passed");return 0;
}
