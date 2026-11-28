#include "controller.h"
static uint32_t magnitude(int64_t n) {
    uint64_t u=(uint64_t)(n<0 ? -n : n);
    return u>UINT32_MAX ? UINT32_MAX : (uint32_t)u;
}
static uint16_t dwell(uint16_t n, bool condition) {
    return !condition ? 0u : n==UINT16_MAX ? n : (uint16_t)(n+1u);
}
static void stop_demand(os_controller *c){
    c->demand=0;c->moving=false;c->still_ms=0;c->live_pid=false;c->requested_rate=0;c->rate_milli=0;c->rate_remainder=0;c->jog_ms=0;os_pid_reset(&c->pid);
}
void os_init(os_controller *c) {
    *c=(os_controller){0};
    /* Simulation defaults only: never interpret as validated motor limits. */
    c->cfg=(os_config){4096,2048,100,800,100,100,OS_MONITOR};
    c->pid_cfg=(os_pid_config){4096,0,0,2,8000};
    c->motion_cfg=(os_motion_config){10000,30000,15000,60000,OS_S_CURVE};
    c->limits=(os_limits){INT32_MIN,INT32_MAX,false};
    c->state=OS_OFF;
}
bool os_config_valid(const os_config *v) {
    return v->following_counts>0u && v->stall_error_counts>0u &&
           v->stall_load_permille<=1000u && v->stall_load_permille>0u &&
           v->dwell_ms>0u && v->clear_ms>0u && v->policy<=OS_PROTECT;
}
bool os_configure(os_controller *c,const os_config *v) {
    if(c->state==OS_ACTIVE || c->state==OS_FAULT || !os_config_valid(v)) return false;
    c->cfg=*v; c->stall_ms=0; c->follow_ms=0; c->healthy_ms=0; return true;
}
bool os_set_pid(os_controller *c,const os_pid_config *v){
    if(c->state!=OS_OFF || !os_pid_config_valid(v))return false;
    c->pid_cfg=*v;os_pid_reset(&c->pid);return true;
}
static bool within(const os_limits *l,int32_t p){return !l->enabled||(p>=l->minimum&&p<=l->maximum);}
bool os_set_limits(os_controller *c,const os_limits *v){
    if(c->state!=OS_OFF||c->latest.enable_input||v->minimum>=v->maximum)return false;
    c->limits=*v;c->healthy_ms=0;return true;
}
bool os_set_live_pid(os_controller *c,const os_pid_config *v){
    if(c->state!=OS_ACTIVE||c->mode!=OS_POSITION||!c->latest.feedback_valid||!c->latest.reference_valid||
       !c->latest.enable_input||c->observed||!os_pid_config_valid(v)||v->output_limit>c->pid_cfg.output_limit)return false;
    c->live_pid_cfg=*v;c->live_pid=true;
    /* Preserve accumulated integral subject to the new output cap. Avoid a derivative kick. */
    int64_t cap=(int64_t)v->output_limit*4096;
    if(c->pid.integral_q12>cap)c->pid.integral_q12=cap;
    if(c->pid.integral_q12< -cap)c->pid.integral_q12= -cap;
    c->pid.previous_actual=c->latest.actual;c->pid.initialized=true;
    return true;
}
bool os_set_motion(os_controller *c,const os_motion_config *v){
    if(c->state!=OS_OFF || !os_motion_config_valid(v))return false;
    c->motion_cfg=*v;return true;
}
bool os_select_mode(os_controller *c, os_mode m) {
    if(c->state!=OS_OFF || m<OS_CNC || m>OS_OPEN_LOOP) return false;
    c->mode=m; return true;
}
bool os_arm(os_controller *c) {
    if(c->state!=OS_OFF || !c->hal_validated || c->faults ||
       c->position_lost || !c->latest.feedback_valid || !c->latest.enable_input ||
       c->observed || c->healthy_ms<c->cfg.clear_ms ||
       (c->mode>OS_JOG) ||
       (c->mode!=OS_CNC && !c->latest.reference_valid) ||
       (c->limits.enabled && (!c->latest.reference_valid || !within(&c->limits,c->latest.actual)))) return false;
    stop_demand(c);c->command=c->latest.actual;c->motion=(os_motion){0};
    c->state=OS_ACTIVE; return true;
}
void os_disable(os_controller *c) {
    stop_demand(c);
    if(c->state!=OS_FAULT) c->state=OS_OFF;
}
bool os_start_move(os_controller *c,int32_t end){
    if(c->state!=OS_ACTIVE || c->mode!=OS_POSITION || c->moving ||
       !c->latest.reference_valid || !c->latest.feedback_valid || !c->latest.enable_input ||
       c->latest.travel_violation || c->latest.speed_counts_s!=0 ||
       magnitude((int64_t)c->command-c->latest.actual)>c->pid_cfg.deadband)return false;
    if(!within(&c->limits,end)||!within(&c->limits,c->latest.actual)||!within(&c->limits,c->command))return false;
    if(!os_motion_plan(&c->motion,c->command,end,&c->motion_cfg))return false;
    c->moving=c->motion.total_ms!=0;return true;
}
static bool rate_mode(const os_controller *c){return c->mode==OS_VELOCITY||c->mode==OS_JOG;}
bool os_set_rate(os_controller *c,int32_t rate,uint16_t lease_ms){
    if(c->state!=OS_ACTIVE||!rate_mode(c)||!c->latest.reference_valid||!c->latest.feedback_valid||
       !c->latest.enable_input||c->latest.travel_violation||magnitude(rate)>c->motion_cfg.speed||
       (c->mode==OS_JOG&&rate&&!lease_ms)||(c->mode==OS_VELOCITY&&lease_ms))return false;
    c->requested_rate=rate;c->jog_ms=rate?lease_ms:0;return true;
}
/* Trapezoidal rate ramp; milli-count/s retains sub-count acceleration.
 * The position servo follows the integrated command, including at zero speed.
 * Decelerate before the commanded boundary, with a one-tick reserve. Actual
 * position violations still trip the controller independently. */
static void rate_tick(os_controller *c){
    int32_t v=c->rate_milli;
    if(c->mode==OS_JOG&&c->jog_ms&&!--c->jog_ms)c->requested_rate=0;
    int32_t goal=c->requested_rate*1000;
    if((v>0&&goal<0)||(v<0&&goal>0))goal=0;
    int32_t direction=v?v:goal;
    int32_t lo=c->limits.enabled?c->limits.minimum:INT32_MIN;
    int32_t hi=c->limits.enabled?c->limits.maximum:INT32_MAX;
    if(direction){
        uint32_t speed=(magnitude(v)+999u)/1000u;
        uint64_t den=2u*(uint64_t)c->motion_cfg.decel;
        uint64_t stop=((uint64_t)speed*speed+den-1u)/den+(speed+999u)/1000u+1u;
        int64_t room=direction>0?(int64_t)hi-c->command:(int64_t)c->command-lo;
        if(room<=0||(uint64_t)room<=stop){goal=0;c->requested_rate=0;}
    }
    uint32_t step=magnitude(goal)>magnitude(v)?c->motion_cfg.accel:c->motion_cfg.decel;
    int64_t diff=(int64_t)goal-v;
    if(diff>(int64_t)step)v+=(int32_t)step;
    else if(diff<-(int64_t)step)v-=(int32_t)step;
    else v=goal;
    int32_t sum=v+c->rate_remainder;
    int64_t pos=(int64_t)c->command+sum/1000000;
    c->rate_remainder=sum%1000000;
    if(pos<=lo||pos>=hi){
        if(pos<lo)pos=lo;
        if(pos>hi)pos=hi;
        if((pos==lo&&v<0)||(pos==hi&&v>0)){v=0;c->rate_remainder=0;c->requested_rate=0;}
    }
    c->command=(int32_t)pos;c->rate_milli=v;c->moving=v!=0||c->requested_rate!=0;
}
void os_tick_1ms(os_controller *c,const os_sample *s) {
    os_sample sample=*s;
    uint32_t critical=sample.critical & OS_CRITICAL;
    if(!sample.feedback_valid) critical|=OS_F_ENCODER;
    if(c->state==OS_ACTIVE && c->mode==OS_POSITION){
        if(!sample.reference_valid)critical|=OS_F_REFERENCE;
        if(!critical && sample.enable_input && c->moving){
            c->command=os_motion_tick(&c->motion);
            c->moving=c->motion.elapsed_ms<c->motion.total_ms;
        }
        sample.target=c->command;
    }
    if(c->state==OS_ACTIVE&&rate_mode(c)){
        if(!sample.reference_valid)critical|=OS_F_REFERENCE;
        if(!critical&&sample.enable_input)rate_tick(c);
        sample.target=c->command;
    }
    if(c->limits.enabled){
        if(c->state==OS_ACTIVE&&!sample.reference_valid)critical|=OS_F_REFERENCE;
        if(sample.reference_valid&&(!within(&c->limits,sample.actual)||!within(&c->limits,sample.target)))sample.travel_violation=true;
    }
    bool unchanged=sample.target==c->latest.target;
    c->latest=sample;
    uint32_t error=magnitude((int64_t)sample.target-sample.actual);
    bool stall=error>=c->cfg.stall_error_counts &&
        magnitude(sample.speed_counts_s)<=c->cfg.stall_speed_counts_s &&
        sample.load_permille>=c->cfg.stall_load_permille;
    bool follow=error>=c->cfg.following_counts;
    bool travel=sample.reference_valid && sample.travel_violation;
    c->stall_ms=dwell(c->stall_ms,c->state==OS_ACTIVE && stall);
    c->follow_ms=dwell(c->follow_ms,c->state==OS_ACTIVE && follow);
    uint32_t noncritical=(c->stall_ms>=c->cfg.dwell_ms?OS_F_STALL:0u) |
        (c->follow_ms>=c->cfg.dwell_ms?OS_F_FOLLOW:0u) |
        (travel?OS_F_TRAVEL:0u);
    c->observed=critical|noncritical;
    c->warnings=critical | (c->cfg.policy>=OS_WARN?noncritical:0u);
    c->healthy_ms=dwell(c->healthy_ms,!critical && !stall && !follow && !travel);
    uint32_t trip=critical;
    if(c->state==OS_ACTIVE && c->cfg.policy==OS_PROTECT) trip|=noncritical;
    if(c->state==OS_ACTIVE && c->limits.enabled && travel)trip|=OS_F_TRAVEL;
    if(trip) {
        if(c->state!=OS_FAULT) c->fault_sample=sample;
        c->faults|=trip;
        if(c->state==OS_ACTIVE || (trip&OS_CRITICAL)) c->position_lost=true;
        c->state=OS_FAULT;stop_demand(c);
    } else if(!sample.enable_input) os_disable(c);
    else if(c->state==OS_ACTIVE){
        c->still_ms=dwell(c->still_ms,unchanged && !c->moving);
        bool stationary=c->still_ms>=100u && magnitude(sample.speed_counts_s)<=c->cfg.stall_speed_counts_s;
        c->demand=os_pid_step(&c->pid,c->live_pid?&c->live_pid_cfg:&c->pid_cfg,sample.target,sample.actual,stationary);
    } else stop_demand(c);
}
bool os_acknowledge(os_controller *c) {
    if(c->state!=OS_FAULT || c->latest.enable_input || c->observed ||
       c->healthy_ms<c->cfg.clear_ms) return false;
    c->faults=0; c->state=OS_OFF; c->stall_ms=0; c->follow_ms=0;
    /* Position loss stays set: future validated reference procedure must clear it. */
    return true;
}
