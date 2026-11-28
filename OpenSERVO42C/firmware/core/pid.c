#include "pid.h"
#define Q 4096LL
#define ERROR_MAX 1000000LL
static int64_t clamp(int64_t v,int64_t max){return v>max?max:v< -max?-max:v;}
bool os_pid_config_valid(const os_pid_config *c){
    return c->kp<=1048576u && c->ki<=1048576u && c->kd<=1048576u &&
        c->deadband<=100000u && c->output_limit>0 && c->output_limit<=32767u;
}
void os_pid_reset(os_pid *p){*p=(os_pid){0};}
int32_t os_pid_step(os_pid *p,const os_pid_config *c,int32_t target,int32_t actual,bool stationary){
    if(!os_pid_config_valid(c)){os_pid_reset(p);return 0;}
    int64_t error=clamp((int64_t)target-actual,ERROR_MAX);
    int64_t delta=p->initialized?clamp((int64_t)actual-p->previous_actual,ERROR_MAX):0;
    p->initialized=true;p->previous_actual=actual;
    int64_t limit=(int64_t)c->output_limit*Q;
    if(stationary && error<=(int64_t)c->deadband && error>=-(int64_t)c->deadband){
        p->integral_q12=0;return 0;
    }
    int64_t pd=error*c->kp-delta*c->kd;
    int64_t next=clamp(p->integral_q12+error*c->ki,limit);
    int64_t output=pd+next;
    if((output<=limit && output>=-limit)||(output>limit && error<0)||(output< -limit && error>0))
        p->integral_q12=next;
    else p->integral_q12=clamp(p->integral_q12,limit);
    return (int32_t)(clamp(pd+p->integral_q12,limit)/Q);
}
