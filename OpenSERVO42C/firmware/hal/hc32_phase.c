#include "phase.h"
/* Original calibration markers and LUT. Never erase or program these pages here. */
bool os_hc32_calibrated_angle(uint16_t raw,uint16_t *angle){
    const volatile uint16_t *knots=(const volatile uint16_t *)(uintptr_t)0x7f94u;
    const volatile uint16_t *lut_valid=(const volatile uint16_t *)(uintptr_t)0x7f96u;
    const uint16_t *lut=(const uint16_t *)(uintptr_t)0x8000u;
    return os_calibrated_angle(lut,*knots,*lut_valid,raw,angle);
}
