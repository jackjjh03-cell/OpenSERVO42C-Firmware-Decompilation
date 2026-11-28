#ifndef OS_TIMING_H
#define OS_TIMING_H
#include <stdint.h>
#include <stdbool.h>
/* SysTick down-counter observation, not a hardware cycle counter. Multiple
 * wraps can coalesce; a recorded value is only a lower bound. */
typedef struct { uint32_t peak, overruns; } os_timing_stat;
static inline bool os_timing_record(os_timing_stat *s,uint32_t period,uint32_t start,uint32_t end,bool wrapped){
    uint32_t elapsed=(wrapped||end>start)?period+start-end:start-end;
    if(elapsed>s->peak)s->peak=elapsed;
    if(elapsed<period)return false;
    if(s->overruns!=UINT32_MAX)s->overruns++;
    return true;
}
#endif
