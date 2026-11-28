#ifndef OS_PHASE_H
#define OS_PHASE_H
#include <stdbool.h>
#include <stdint.h>
#define OS_LUT_ENTRIES 16384u
/* Valid flags are the original V1.1.3 16-bit markers. No writes. */
bool os_calibrated_angle(const uint16_t *lut,uint16_t knots_valid,uint16_t lut_valid,uint16_t raw,uint16_t *angle);
uint16_t os_electrical_phase(uint16_t angle,uint16_t previous,bool predict,int32_t demand,uint8_t geometry);
int16_t os_sine_q14(uint16_t phase);
/* Hardware adapter, read only. Caller must validate the LUT before use. */
bool os_hc32_calibrated_angle(uint16_t raw,uint16_t *angle);
#endif
