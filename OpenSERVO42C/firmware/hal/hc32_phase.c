#include "phase.h"
/* Caller must first validate the table against its persistent calibration state. */
bool os_hc32_calibrated_angle(uint16_t raw,uint16_t *angle){
    const uint16_t *lut=(const uint16_t *)(uintptr_t)0x8000u;
    return os_calibrated_angle(lut,1,1,raw,angle);
}
#include "calibration_check.h"
os_calibration_check os_hc32_check_calibration(uint8_t state,uint32_t crc){
    return os_check_managed_calibration((const uint16_t *)(uintptr_t)0x8000u,state,crc,
        *(volatile uint16_t *)(uintptr_t)0x7f94u,*(volatile uint16_t *)(uintptr_t)0x7f96u);
}
