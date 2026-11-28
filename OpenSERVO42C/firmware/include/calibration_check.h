#ifndef OS_CALIBRATION_CHECK_H
#define OS_CALIBRATION_CHECK_H
#include <stdbool.h>
#include <stdint.h>
/* Circular 16-bit inverse mapping: one signed turn per raw encoder turn. */
typedef struct {int32_t winding;uint16_t max_delta,forward,reverse,flat;uint8_t flags;} os_calibration_check;
enum {OS_CAL_MARKERS=1,OS_CAL_WINDING=2,OS_CAL_AMBIGUOUS=4};
os_calibration_check os_check_calibration(const uint16_t *lut,uint16_t knots,uint16_t valid);
os_calibration_check os_hc32_check_calibration(void);
#endif
