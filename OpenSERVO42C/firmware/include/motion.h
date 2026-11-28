#ifndef OS_MOTION_H
#define OS_MOTION_H
#include <stdbool.h>
#include <stdint.h>
/* Rest-to-rest standalone moves only. Units: calibrated position counts, s. */
typedef enum { OS_TRAPEZOID=0, OS_S_CURVE=1 } os_curve;
typedef struct { uint32_t speed, accel, decel, jerk; uint8_t curve; } os_motion_config;
typedef struct {
    int32_t start, end, position;
    uint32_t distance, up_ms, cruise_ms, down_ms, total_ms, elapsed_ms;
    uint8_t curve;
} os_motion;
bool os_motion_config_valid(const os_motion_config *c);
bool os_motion_plan(os_motion *m,int32_t start,int32_t end,const os_motion_config *c);
int32_t os_motion_position(const os_motion *m,uint32_t ms);
int32_t os_motion_tick(os_motion *m);
#endif
