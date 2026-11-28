#include "peripherals.h"
/* select one of four five-bit channel fields. */
uint32_t adc_select_channel(uint32_t slot, uint32_t channel) {
    if (slot>3)return 4;
    unsigned shift=slot*5;
    RAM32(0x4000244Cu)=(RAM32(0x4000244Cu)&~(31u<<shift))|((channel&31u)<<shift);
    return 0;
}
void adc_clear_flags(uint32_t mask) {
    ADC_AUX_REG(0x34)&=~mask;
}
void adc_enable(void) {
    ADC_REG(4)|=1;
}
uint32_t adc_test_flags(uint32_t mask) {
    return (ADC_AUX_REG(0x30)&mask)!=0;
}
uint32_t adc_read_result(uint32_t index) {
    return RAM32(ADC_RESULT_BASE+index*4);
}
void adc_start(void) {
    RAM32(0x400024C8)=1;
}
void adc_stop(void) {
    RAM32(0x400024C8)=0;
}
void adc_interrupt_mask(uint32_t mask, uint32_t enable) {
    if (enable==1)ADC_AUX_REG(0x3c)|=mask;
    else ADC_AUX_REG(0x3c)&=~mask;
}
uint32_t adc_config_sequence(uint32_t config) {
    if (!config||RAM8(config)>4)return 4;
    ADC_REG(8)=(ADC_REG(8)&~0x200u)|((RAM8(config+1)&1u)<<9);
    ADC_REG(0x4c)=(ADC_REG(0x4c)&~0x300000u)|(((RAM8(config)-1u)&3u)<<20);
    return 0;
}
uint32_t register_config_50(uint32_t base, uint32_t config) {
    if (!config||RAM8(config)>5)return 4;
    RAM32(base+0x50)=(RAM32(base+0x50)&~14u)|((RAM8(config)&7u)<<1);
    RAM32(base+0x50)=(RAM32(base+0x50)&~256u)|((RAM8(config+1)&1u)<<8);
    RAM32(base+0x50)=(RAM32(base+0x50)&~112u)|((RAM8(config+2)&7u)<<4);
    return 0;
}
uint32_t register_write_low16_or(uint32_t base, uint32_t value) {
    RAM32(base)=(RAM32(base)&0xffff0000u)|value;
    return 0;
}
uint32_t register_write_offset4(uint32_t base, uint32_t value) {
    RAM32(base+4)=value;
    return 0;
}
uint32_t register_start_50(uint32_t base) {
    RAM32(base+0x50)|=1;
    return 0;
}
uint32_t register_stop_50(uint32_t base) {
    RAM32(base+0x50)&=~1u;
    return 0;
}
void timer_clear_all(uint32_t t) {
    TIMER_REG(t, 0x14)=0;
}
uint32_t timer_clear_flag(uint32_t t, uint32_t bit) {
    bit&=255u;
    TIMER_REG(t, 0x14)=~(bit<32?(1u<<bit):0u);
    return 0;
}
uint32_t timer_test_flag(uint32_t t, uint32_t bit) {
    uint32_t v=TIMER_REG(t, 0x10);
    bit&=255u;
    return bit<32?(v>>bit)&1u:0;
}
uint32_t timer_write_count(uint32_t t, uint32_t value) {
    TIMER_REG(t, 0)=(TIMER_REG(t, 0)&0xffff0000u)|value;
    return 0;
}
uint32_t factory_timer_reload(uint32_t t, uint32_t value) {
    TIMER_REG(t, 4)=(TIMER_REG(t, 4)&0xffff0000u)|value;
    return 0;
}
uint32_t factory_timer_start(uint32_t t) {
    TIMER_REG(t, 0xc)|=1;
    return 0;
}
uint32_t factory_timer_stop(uint32_t t) {
    TIMER_REG(t, 0xc)&=~1u;
    return 0;
}
uint32_t timer_write_count_mode(uint32_t t, uint32_t value, uint32_t mode) {
    TIMER_REG(t, 0)=(TIMER_REG(t, 0)&0xffff0000u)|value;
    TIMER_REG(t, 0xc)=(TIMER_REG(t, 0xc)&~128u)|((mode&1u)<<7);
    return 0;
}
uint32_t timer_write_compare(uint32_t t, uint32_t channel, uint32_t value) {
    if (channel>1)return 1;
    uint32_t a=TIMER_BASE+t*TIMER_STRIDE+0x3c+channel*4;
    RAM32(a)=(RAM32(a)&0xffff0000u)|value;
    return 0;
}
uint32_t timer_reload_alias(uint32_t t, uint32_t value) {
    TIMER_REG(t, 4)=(TIMER_REG(t, 4)&0xffff0000u)|value;
    return 0;
}
uint32_t timer_config_30(uint32_t t, uint32_t a, uint32_t b) {
    TIMER_REG(t, 0x30)=(TIMER_REG(t, 0x30)&~0x1000u)|((a&1u)<<12);
    TIMER_REG(t, 0x30)=(TIMER_REG(t, 0x30)&~0x800u)|((b&1u)<<11);
    return 0;
}
#define FIELD(t,off,mask,shift,v) TIMER_REG(t,off)=(TIMER_REG(t,off)&~((uint32_t)(mask)<<(shift)))|(((uint32_t)(v)&(mask))<<(shift))
uint32_t timer_config_18(uint32_t t, uint32_t config) {
    FIELD(t, 0x18, 1, 4, RAM8(config));
    FIELD(t, 0x18, 7, 0, RAM8(config+1));
    FIELD(t, 0x18, 7, 8, RAM8(config+2));
    FIELD(t, 0x18, 7, 5, RAM8(config+3));
    return 0;
}
uint32_t timer_config_capture(uint32_t t, uint32_t config) {
    FIELD(t, 0x24, 1, 4, 0);
    FIELD(t, 0x1c, 7, 0, RAM8(config+1));
    FIELD(t, 0x1c, 1, 3, RAM8(config+2));
    FIELD(t, 0x24, 1, 6, RAM8(config+3));
    FIELD(t, 0xc, 3, 21, RAM8(config+4));
    FIELD(t, 0x24, 1, 5, 0);
    FIELD(t, 0x1c, 7, 4, RAM8(config+6));
    FIELD(t, 0x1c, 1, 7, RAM8(config+7));
    FIELD(t, 0x24, 1, 7, RAM8(config+8));
    FIELD(t, 0x24, 3, 12, RAM8(config+9));
    return 0;
}
uint32_t timer_write_34(uint32_t t, uint32_t value) {
    TIMER_REG(t, 0x34)=(TIMER_REG(t, 0x34)&0xffffff00u)|value;
    return 0;
}
uint32_t timer_config_20(uint32_t t, uint32_t config) {
    FIELD(t, 0x20, 1, 7, RAM8(config));
    FIELD(t, 0x20, 1, 0, RAM8(config+1));
    FIELD(t, 0x20, 1, 1, RAM8(config+2));
    FIELD(t, 0x20, 1, 4, RAM8(config+3));
    return 0;
}
uint32_t timer_set_400(uint32_t t) {
    TIMER_REG(t, 0xc)|=0x400;
    return 0;
}
uint32_t timer_config_basic(uint32_t t, uint32_t config) {
    FIELD(t, 0xc, 3, 12, RAM8(config));
    FIELD(t, 0xc, 1, 9, RAM8(config+1));
    FIELD(t, 0xc, 1, 8, RAM8(config+2));
    FIELD(t, 0xc, 7, 4, RAM8(config+3));
    FIELD(t, 0xc, 1, 3, RAM8(config+4));
    FIELD(t, 0xc, 1, 2, RAM8(config+5));
    FIELD(t, 0xc, 1, 1, RAM8(config+6));
    return 0;
}
uint32_t timer_config_extended(uint32_t t, uint32_t config) {
    FIELD(t, 0xc, 3, 12, RAM8(config));
    FIELD(t, 0xc, 7, 4, RAM8(config+2));
    FIELD(t, 0xc, 1, 2, RAM8(config+3));
    FIELD(t, 0xc, 1, 1, RAM8(config+4));
    FIELD(t, 0xc, 1, 3, RAM8(config+5));
    FIELD(t, 0xc, 1, 14, RAM8(config+6));
    FIELD(t, 0xc, 1, 17, RAM8(config+7));
    FIELD(t, 0xc, 1, 27, RAM8(config+1));
    return 0;
}
/* original UART transmit completion. */
void factory_uart_tx_end(void) {
    RAM32(0x40000014)&=~1u;
    RAM32(0x40000014)&=~2u;
    RAM32(0x40000004)&=~2u;
    RAM32(0x40000004)|=1u;
}
