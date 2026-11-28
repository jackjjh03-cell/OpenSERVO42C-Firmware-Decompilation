#include "hc32_inhibit.h"
#include <stdint.h>
#define REG(a) (*(volatile uint32_t *)(uintptr_t)(a))
/* Gate/ADC disable and compare reset from original remaining_helpers.c.
 * Inhibit both timer outputs before touching compare values. No enable here.
 * Physical levels and shutdown latency must be checked on the stock board. */
void os_hc32_inhibit(void){
    uint32_t mask;
    __asm volatile("mrs %0, primask\ncpsid i" : "=r"(mask) :: "memory");
    REG(0x40000c30u)&=~0x1000u;
    REG(0x40000d30u)&=~0x1000u;
    REG(0x40021014u)&=~0x80000u;
    REG(0x40021014u)&=~0x70000u;
    REG(0x400024b4u)&=~0x20u;
    REG(0x40000c3cu)=(REG(0x40000c3cu)&0xffff0000u)|1200u;
    REG(0x40000c40u)=(REG(0x40000c40u)&0xffff0000u)|1200u;
    REG(0x40000d3cu)=(REG(0x40000d3cu)&0xffff0000u)|1200u;
    REG(0x40000d40u)=(REG(0x40000d40u)&0xffff0000u)|1200u;
    __asm volatile("dsb\nmsr primask, %0" :: "r"(mask) : "memory");
}
/* Factory indicator output; not confirmed to be an external fault connector. */
void os_hc32_indicator(bool active){
    if(active)REG(0x40020d88u)|=0x4000u;
    else REG(0x40020d88u)&=~0x4000u;
}
