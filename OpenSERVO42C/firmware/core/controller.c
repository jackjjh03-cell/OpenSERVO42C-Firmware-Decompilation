#include "controller.h"
static uint32_t magnitude(int64_t n) {
    uint64_t u=(uint64_t)(n<0 ? -n : n);
    return u>UINT32_MAX ? UINT32_MAX : (uint32_t)u;
}
static uint16_t dwell(uint16_t n, bool condition) {
    return !condition ? 0u : n==UINT16_MAX ? n : (uint16_t)(n+1u);
}
void os_init(os_controller *c) {
    *c=(os_controller){0};
    /* Simulation defaults only: never interpret as validated motor limits. */
    c->cfg=(os_config){4096,2048,100,800,100,100,OS_MONITOR};
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
bool os_select_mode(os_controller *c, os_mode m) {
    if(c->state!=OS_OFF || m<OS_CNC || m>OS_OPEN_LOOP) return false;
    c->mode=m; return true;
}
bool os_arm(os_controller *c) {
    if(c->state!=OS_OFF || !c->hal_validated || c->faults ||
       c->position_lost || !c->latest.feedback_valid || !c->latest.enable_input ||
       c->observed || c->healthy_ms<c->cfg.clear_ms || c->mode==OS_TORQUE) return false;
    c->state=OS_ACTIVE; return true;
}
void os_disable(os_controller *c) {
    if(c->state!=OS_FAULT) c->state=OS_OFF;
}
void os_tick_1ms(os_controller *c,const os_sample *s) {
    c->latest=*s;
    uint32_t critical=s->critical & OS_CRITICAL;
    if(!s->feedback_valid) critical|=OS_F_ENCODER;
    uint32_t error=magnitude((int64_t)s->target-s->actual);
    bool stall=error>=c->cfg.stall_error_counts &&
        magnitude(s->speed_counts_s)<=c->cfg.stall_speed_counts_s &&
        s->load_permille>=c->cfg.stall_load_permille;
    bool follow=error>=c->cfg.following_counts;
    bool travel=s->reference_valid && s->travel_violation;
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
    if(trip) {
        if(c->state!=OS_FAULT) c->fault_sample=*s;
        c->faults|=trip;
        if(c->state==OS_ACTIVE || (trip&OS_CRITICAL)) c->position_lost=true;
        c->state=OS_FAULT;
    } else if(!s->enable_input) os_disable(c);
}
bool os_acknowledge(os_controller *c) {
    if(c->state!=OS_FAULT || c->latest.enable_input || c->observed ||
       c->healthy_ms<c->cfg.clear_ms) return false;
    c->faults=0; c->state=OS_OFF; c->stall_ms=0; c->follow_ms=0;
    /* Position loss stays set: future validated reference procedure must clear it. */
    return true;
}
