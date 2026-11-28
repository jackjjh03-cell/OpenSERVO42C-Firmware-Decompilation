#ifndef OS_PID_H
#define OS_PID_H
#include <stdbool.h>
#include <stdint.h>
/* Q12 gains per 1 ms sample. Output is a normalized signed demand, NOT amps. */
typedef struct { uint32_t kp,ki,kd,deadband; uint16_t output_limit; } os_pid_config;
typedef struct { int64_t integral_q12; int32_t previous_actual; bool initialized; } os_pid;
bool os_pid_config_valid(const os_pid_config *c);
void os_pid_reset(os_pid *p);
int32_t os_pid_step(os_pid *p,const os_pid_config *c,int32_t target,int32_t actual,bool stationary);
#endif
