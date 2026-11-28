#ifndef OS_CURRENT_H
#define OS_CURRENT_H
#include <stdbool.h>
#include <stdint.h>
#define OS_CURRENT_OFFSET_SAMPLES 18u
#define OS_PWM_PERIOD 1200u
typedef struct { uint32_t sum[2]; uint16_t min[2],max[2]; uint8_t count; bool invalid; } os_offset_cal;
typedef struct { uint16_t slot2,slot1; } os_offsets;
typedef struct { int32_t a,b; } os_pair;
typedef struct { uint16_t a0,a1,b0,b1; } os_pwm_compare;
void os_offset_begin(os_offset_cal *c);
bool os_offset_add(os_offset_cal *c,uint16_t slot2,uint16_t slot1);
bool os_offset_finish(const os_offset_cal *c,os_offsets *out);
bool os_current_feedback(const os_offsets *o,uint16_t slot2,uint16_t slot1,os_pair *out);
/* sin/cos Q14 from calibrated electrical phase. Commands and feedback in raw ADC units. */
bool os_current_step(os_pair feedback,os_pair target,int16_t sin_q14,int16_t cos_q14,
                     uint16_t gain_q11,uint16_t scale_raw,uint16_t compare_limit,
                     os_pwm_compare *out);
void os_pwm_inactive(os_pwm_compare *out);
/* Hardware-only adapter, requires configured ADC and timers. */
bool os_hc32_adc_snapshot(uint16_t *scale,uint16_t *slot2,uint16_t *slot1);
bool os_hc32_write_compare(const os_pwm_compare *p);
#endif
