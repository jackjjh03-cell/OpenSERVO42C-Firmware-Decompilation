#ifndef OS_HC32_INHIBIT_H
#define OS_HC32_INHIBIT_H
#include <stdbool.h>
/* Peripheral clocks must already be initialized. Caller must latch software
 * faults and prevent later enable/ISR writes; this is not a hardware interlock. */
void os_hc32_inhibit(void);
void os_hc32_indicator(bool active);
#endif
