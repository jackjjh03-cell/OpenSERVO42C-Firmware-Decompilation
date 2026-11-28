#include "motion.h"
#define MAX_DISTANCE 1000000u
#define MAX_TIME 600000u
#define Q 1073741824LL
static uint64_t ceil_div(uint64_t n,uint64_t d){return n/d+(n%d!=0u);}
static uint32_t ramp(uint32_t v,uint32_t a){return (uint32_t)ceil_div((uint64_t)v*1000u,a);}
bool os_motion_config_valid(const os_motion_config *c){
    return c->speed>0 && c->speed<=1000000u && c->accel>0 && c->accel<=100000000u &&
        c->decel>0 && c->decel<=100000000u && c->jerk>0 && c->jerk<=1000000000u && c->curve<=OS_S_CURVE;
}
/* Quintic smoothstep: v<=2D/T, |a|<=6D/T^2, |j|<=60D/T^3.
 * Conservative duration; both acceleration limits enforced using the smaller.
 * No floating point. Planning runs outside the current-control ISR. */
static bool smooth_fits(uint32_t d,uint32_t t,const os_motion_config *c){
    uint32_t a=c->accel<c->decel?c->accel:c->decel;
    uint64_t t2=(uint64_t)t*t;
    return t>=ceil_div((uint64_t)d*2000u,c->speed) &&
        t2>=ceil_div((uint64_t)d*6000000u,a) &&
        t2*t>=ceil_div((uint64_t)d*60000000000ULL,c->jerk);
}
bool os_motion_plan(os_motion *m,int32_t start,int32_t end,const os_motion_config *c){
    if(!os_motion_config_valid(c))return false;
    int64_t delta=(int64_t)end-start;
    uint64_t d=(uint64_t)(delta<0?-delta:delta);
    if(d>MAX_DISTANCE)return false;
    os_motion p={.start=start,.end=end,.position=start,.distance=(uint32_t)d,.curve=c->curve};
    if(!d){*m=p;return true;}
    if(c->curve==OS_S_CURVE){
        if(!smooth_fits(p.distance,MAX_TIME,c))return false;
        uint32_t lo=1,hi=MAX_TIME;
        while(lo<hi){uint32_t t=lo+(hi-lo)/2u;if(smooth_fits(p.distance,t,c))hi=t;else lo=t+1u;}
        p.total_ms=lo;
    }else{
        uint32_t lo=1,hi=c->speed;
        /* Highest integer peak whose two rounded ramps fit the distance. */
        while(lo<hi){uint32_t v=lo+(hi-lo+1u)/2u;
            uint64_t area=(uint64_t)v*(ramp(v,c->accel)+ramp(v,c->decel));
            if(area<=d*2000u)lo=v;else hi=v-1u;
        }
        p.up_ms=ramp(lo,c->accel);p.down_ms=ramp(lo,c->decel);
        uint64_t span=ceil_div(d*2000u,lo), ramps=(uint64_t)p.up_ms+p.down_ms;
        p.cruise_ms=span>ramps?(uint32_t)ceil_div(span-ramps,2u):0u;
        uint64_t total=ramps+p.cruise_ms;
        if(total>MAX_TIME)return false;
        p.total_ms=(uint32_t)total;
    }
    *m=p;return true;
}
int32_t os_motion_position(const os_motion *m,uint32_t ms){
    if(ms>=m->total_ms)return m->end;
    uint64_t d=m->distance, x;
    if(m->curve==OS_S_CURVE){
        bool mirror=ms>m->total_ms/2u;
        uint32_t t=mirror?m->total_ms-ms:ms;
        int64_t u=(int64_t)t*Q/m->total_ms;
        int64_t u2=u*u/Q,u3=u2*u/Q;
        int64_t f=u3*(10*Q-15*u+6*u2)/Q;
        if(f<0)f=0;
        if(f>Q)f=Q;
        if(mirror)f=Q-f;
        x=(d*(uint64_t)f+(uint64_t)Q/2u)/(uint64_t)Q;
    }else{
        uint64_t den=(uint64_t)m->up_ms+2u*m->cruise_ms+m->down_ms;
        if(ms<m->up_ms)x=d*ms*ms/((uint64_t)m->up_ms*den);
        else if(ms<m->up_ms+m->cruise_ms)x=d*(2u*(uint64_t)ms-m->up_ms)/den;
        else {uint64_t left=m->total_ms-ms;x=d-d*left*left/((uint64_t)m->down_ms*den);}
    }
    return (int32_t)((int64_t)m->start+(m->end>=m->start?(int64_t)x:-(int64_t)x));
}
int32_t os_motion_tick(os_motion *m){
    if(m->elapsed_ms<m->total_ms)m->elapsed_ms++;
    /* Guard sub-count fixed-point rounding at segment boundaries. */
    int32_t next=os_motion_position(m,m->elapsed_ms);
    if(m->end>=m->start?next>=m->position:next<=m->position)m->position=next;
    return m->position;
}
