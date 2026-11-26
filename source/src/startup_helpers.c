#include "factory_layout.h"
extern void factory_delay_100us(uint32_t);
extern uint32_t factory_divide(uint32_t, uint32_t);
extern void uart_reset_rx(void);
/* 0x0758 ADC configuration with raw bit masks, matching ordered MMIO. */
uint32_t adc_startup_config(uint32_t cfg) {
    if (!cfg)return 4;
    RAM32(0x40002404u)=1;
    factory_delay_100us(1);
    RAM32(0x40002404u)=RAM8(cfg+1)|RAM16(cfg+4)|RAM16(cfg+2)|RAM16(cfg+6)|RAM16(cfg+8)|RAM32(0x40002404u);
    RAM32(0x40002408u)=(RAM32(0x40002408u)&~0x400u)|((RAM8(cfg)&1u)<<10);
    RAM32(0x40002408u)=(RAM32(0x40002408u)&~4u)|((RAM8(cfg+10)&1u)<<2);
    return 0;
}
void adc_power_on(void) {
    RAM32(0x40002400u)|=1u;
    factory_delay_100us(1);
}
/* preserve the original direction-dependent register order. */
void factory_apply_direction(void) {
    RAM32(0x40003088u)=0;
    RAM32(0x4000308cu)=0;
    if (RAM8(WORK_RAM(0x0730u))) {
        RAM32(0x40003088u)|=0x40;
        RAM32(0x4000308cu)|=0x10;
    }
    else {
        RAM32(0x40003088u)|=0x10;
        RAM32(0x4000308cu)|=0x40;
    }
}
void uart_disable(void) {
    RAM32(0x40000004u)&=~2u;
    RAM32(0x40000004u)&=~1u;
    RAM32(0x40000014u)&=~2u;
    RAM32(0x40000014u)&=~1u;
    uart_reset_rx();
}
/* 0x3052 formats only up to four positions, zero produces an empty string.
 * Preserve legacy truncation to a byte of an oversized first digit. */
void factory_format_uint(uint32_t v, uint32_t dst) {
    unsigned n=0;
    if (v) {
        uint32_t scale=v>=1000?1000:v>=100?100:v>=10?10:1;
        for (;;) {
            uint32_t q=factory_divide(v, scale);
            RAM8(dst+n++)=(uint8_t)(q+48u);
            v-=q*scale;
            if (scale==1)break;
            scale=factory_divide(scale, 10);
        }
    }
    while (n<9)RAM8(dst+n++)=0;
}
extern uint32_t factory_clock_frequency(void);
uint32_t peripheral_clock_frequency(void) {
    uint32_t f=factory_clock_frequency();
    return f>>((RAM32(0x40002000u)&0x1fffu)>>11);
}
void update_system_clock(void) {
    RAM32(WORK_RAM(0x0000u))=factory_clock_frequency();
}
