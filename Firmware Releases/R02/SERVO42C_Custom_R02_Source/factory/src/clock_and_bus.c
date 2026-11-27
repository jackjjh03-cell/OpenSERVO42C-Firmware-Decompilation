#include "factory_layout.h"
extern void system_key_sequence(void), factory_delay_100us(uint32_t);
extern uint32_t gpio_set_direction_bit(uint32_t, uint32_t), gpio_clear_output_bit(uint32_t, uint32_t), gpio_config_pin(uint32_t, uint32_t, uint32_t), gpio_write_pin_register(uint32_t, uint32_t, uint32_t), spi_config_port(uint32_t, uint32_t);
/* original clock readiness polling and restoration of saved control. */
uint32_t clock_select_wait(uint32_t source, uint32_t enable) {
    const uint32_t base=0x40002000u;
    uint32_t saved=RAM32(base), status=0;
    system_key_sequence();
    enable=!!enable;
    uint32_t reg=0, mask=0;
    switch (source) {
    case 0:reg=base+12;
        mask=0x800;
        break;
    case 1:RAM32(0x40020dccu)|=3;
        reg=base+16;
        mask=0x40;
        break;
    case 2:reg=base+20;
        mask=0x1000;
        break;
    case 3:RAM32(0x40020d8cu)|=0xc000;
        reg=base+24;
        mask=0x40;
        break;
    case 4:RAM32(base+32)|=0x10000;
        RAM32(0x40002400u)|=1;
        factory_delay_100us(1);
        reg=base+60;
        mask=0x40000;
        break;
    default:status=4;
        break;
    }
    if (!status) {
        uint32_t bit=1u<<source;
        RAM32(base)=(RAM32(base)&~bit)|(enable?bit:0);
        if (enable)while (!(RAM32(reg)&mask)) {
        }
    }
    RAM32(base)=saved;
    return status;
}
/* oscillator trim frequency selection and clock divider. */
static inline __attribute__((always_inline)) uint32_t rc_frequency(const uint16_t *trim) {
    for (unsigned i=0;i<4;i++)if ((RAM32(0x4000200cu)&2047u)==trim[i])return RAM32(ASSET_ADDR(oscillator_frequencies, 0xcu)-4*i);
    return 4000000u;
}
uint32_t factory_clock_frequency(void) {
    uint16_t trim[5];
    for (unsigned i=0;i<4;i++)trim[i]=RAM16(0x00100c00u+2*i);
    trim[4]=RAM16(0x00100c20u);
    uint32_t freq=0, base=0x40002000u;
    switch ((RAM32(base)>>5)&7u) {
    case 0:freq=rc_frequency(trim);
        break;
    case 1:freq=32000000u;
        break;
    case 2:freq=trim[4]==(RAM32(base+20)&1023u)?38400u:32768u;
        break;
    case 3:freq=32768u;
        break;
    case 4:freq=32000000u;
        if ((RAM32(base+60)&3u)==3)freq=rc_frequency(trim);
        freq*=((RAM32(base+60)>>5)&15u);
        break;
    default:break;
    }
    return freq>>((RAM32(base)>>8)&7u);
}
/* the factory board-init caller leaves r2=1 after enabling
 * clocks. The legacy routine clears its low byte, so all six GPIO fields
 * are zero except field 5. Do not read an unspecified C argument. */
void initialize_indicator(void) {
    uint8_t cfg[6]={0, 0, 0, 0, 0, 1};
    gpio_set_direction_bit(128, 14);
    gpio_config_pin(128, 14, (uint32_t)(uintptr_t)cfg);
}
/* original SPI pin and peripheral configuration sequence. */
void initialize_encoder_bus(void) {
 /* The preceding factory display setup returns r2=1. After clearing
  * its low byte the legacy configuration has the following values. */
    uint8_t cfg[6]={0, 0, 0, 0, 0, 1};
    uint32_t p=(uint32_t)(uintptr_t)cfg;
    for (uint32_t pin=4;pin<=7;pin++) {
        if (pin==6)continue;
        if (pin<6)gpio_clear_output_bit(0, pin);
        gpio_config_pin(0, pin, p);
        gpio_write_pin_register(0, pin, 1);
    }
    cfg[0]=cfg[1]=cfg[2]=1;
    gpio_config_pin(0, 6, p);
    gpio_write_pin_register(0, 6, 1);
    uint32_t spi=0x04080210u;
    spi_config_port(0x40000800u, (uint32_t)(uintptr_t)&spi);
}
/* I2C controller clock divider, own address and reset sequence. */
extern uint32_t factory_divide(uint32_t, uint32_t), reg_set_bit_plus8(uint32_t, uint32_t), reg_write_plus4(uint32_t, uint32_t);
uint32_t i2c_configure(uint32_t base, uint32_t cfg) {
    RAM32(0x40002028u)&=~16u;
    RAM32(0x40002028u)|=16u;
    RAM32(base+8)=0;
    RAM32(base+8)=RAM8(cfg+8);
    uint32_t baud=RAM32(cfg+4), clk=RAM32(cfg);
    if ((baud<<4)>clk)return 1;
    if (RAM8(cfg+8)!=0x40) {
        RAM32(base)=0;
        uint8_t a=(uint8_t)((RAM8(cfg+9)<<1)|RAM8(cfg+10));
        RAM8(cfg+9)=a;
        RAM32(base+16)=a;
        return 1;
    }
    RAM32(base)=1;
    baud=RAM32(cfg+4);
    clk=RAM32(cfg);
    uint8_t div=(uint8_t)((factory_divide(clk, baud)>>3)-1u);
    if (div<9)reg_set_bit_plus8(base, 0);
    return reg_write_plus4(base, div);
}
