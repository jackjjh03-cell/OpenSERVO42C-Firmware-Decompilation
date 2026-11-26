#include "factory_layout.h"
extern uint32_t gpio_config_pin(uint32_t, uint32_t, uint32_t);
extern uint32_t gpio_read_input_bit(uint32_t, uint32_t);
extern void fault_indicator_write(uint32_t);
extern void system_key_sequence(void), update_system_clock(void);
extern uint32_t factory_divide(uint32_t, uint32_t);
/* preserve system unlock and pin configuration order. */
void factory_config_pins(void) {
    RAM32(0x40002008u)=0x5a5au;
    RAM32(0x40002008u)=0xa5a5u;
    RAM32(0x40002004u)|=0x100u;
    uint8_t cfg[6]={1, 1, 0, 0, 0, 0};
    gpio_config_pin(0, 13, (uint32_t)(uintptr_t)cfg);
    gpio_config_pin(0, 14, (uint32_t)(uintptr_t)cfg);
}
/* retain factory boot interlock when PA9 is low. */
void initialize_buttons(void) {
    uint8_t cfg[6]={1, 1, 1, 0, 0, 1};
    gpio_config_pin(0, 9, (uint32_t)(uintptr_t)cfg);
    gpio_config_pin(0, 10, (uint32_t)(uintptr_t)cfg);
    gpio_config_pin(64, 4, (uint32_t)(uintptr_t)cfg);
    if (!gpio_read_input_bit(0, 9)) {
        fault_indicator_write(1);
        for (;;) {
        }
    }
}
#define SET_FIELD(a,mask,value) (RAM32(a)=(RAM32(a)&~(uint32_t)(mask))|(uint32_t)(value))
/* configuration order is observable in peripheral registers. */
uint32_t timer2_configure(uint32_t cfg) {
    const uint32_t a=0x4000580cu;
    SET_FIELD(a, 0x3000, (RAM8(cfg)&3u)<<12);
    SET_FIELD(a, 0x200, (RAM8(cfg+1)&1u)<<9);
    SET_FIELD(a, 0x100, (RAM8(cfg+2)&1u)<<8);
    SET_FIELD(a, 0x70, (RAM8(cfg+3)&7u)<<4);
    SET_FIELD(a, 8, (RAM8(cfg+4)&1u)<<3);
    SET_FIELD(a, 4, (RAM8(cfg+5)&1u)<<2);
    SET_FIELD(a, 2, (RAM8(cfg+6)&1u)<<1);
    return 0;
}
uint32_t set_clock_divider(uint32_t v) {
    system_key_sequence();
    SET_FIELD(0x40002000u, 0xe0, (v&7u)<<5);
    update_system_clock();
    return 0;
}
/* Preserve SysTick polling and reload, including zero-duration behavior. */
static inline __attribute__((always_inline)) void delay_ticks(uint32_t n, uint32_t divisor) {
    RAM32(0xe000e014u)=0xffffffu;
    RAM32(0xe000e018u)=0;
    RAM32(0xe000e010u)=5;
    uint32_t clock=RAM32(WORK_RAM(0x0000u));
    while (n--) {
        RAM32(0xe000e018u)=0;
        uint32_t threshold=0x1000000u-factory_divide(clock, divisor);
        while (RAM32(0xe000e018u)>threshold) {
        }
    }
    RAM32(0xe000e010u)&=~1u;
}
void factory_delay_100us(uint32_t n) {
    delay_ticks(n, 10000);
}
void factory_delay_ms(uint32_t n) {
    delay_ticks(n, 1000);
}
/* startup port masks; do not change original output levels. */
void initialize_port_masks(void) {
    uint32_t saved=RAM32(0x40002020u);
    RAM32(0x40002020u)|=0x10000000u;
    RAM32(0x40020d0cu)&=0xfff4u;
    RAM32(0x40020d4cu)&=0x08fbu;
    RAM32(0x40020d8cu)&=0xc000u;
    RAM32(0x40020dccu)&=0xff0bu;
    RAM32(0x40020d00u)|=0xbu;
    RAM32(0x40020d40u)|=0xf704u;
    RAM32(0x40020d80u)|=0x3fffu;
    RAM32(0x40020dc0u)|=0xf4u;
    RAM32(0x40020d20u)|=0xbu;
    RAM32(0x40020d60u)|=0xf704u;
    RAM32(0x40020da0u)|=0x3fffu;
    RAM32(0x40020de0u)|=0xf4u;
    RAM32(0x40002020u)=saved;
}
/* clock configuration, preserving ROM trim checks and return codes. */
uint32_t configure_clock_source(uint32_t cfg) {
    uint16_t trims[4];
    for (unsigned i=0;i<4;i++)trims[i]=RAM16(0x00100c00u+2*i);
    const uint32_t r=0x4000203cu;
    if (RAM8(cfg+2)!=3) {
        if ((uint32_t)(32000000u*RAM8(cfg+3))>48000000u)return 6;
    }
    else {
        if (trims[0]==(RAM32(0x4000200cu)&0x7ffu)&&RAM8(cfg+3)>2)return 6;
        if (trims[1]==(RAM32(0x4000200cu)&0x7ffu)&&RAM8(cfg+3)>2)return 6;
        if (trims[2]==(RAM32(0x4000200cu)&0x7ffu)&&RAM8(cfg+3)>3)return 6;
        if (trims[3]==(RAM32(0x4000200cu)&0x7ffu)&&RAM8(cfg+3)>6)return 6;
    }
    SET_FIELD(r, 0x6000, (RAM8(cfg)&3u)<<13);
    SET_FIELD(r, 0x1c, (RAM8(cfg+1)&7u)<<2);
    SET_FIELD(r, 0x1e0, (RAM8(cfg+3)&15u)<<5);
    SET_FIELD(r, 3, RAM8(cfg+2)&3u);
    return 0;
}
extern uint32_t flash_config_mode(uint32_t), system_field15(uint32_t), clock_select_wait(uint32_t, uint32_t);
/* System clock trim and startup sequence. */
void factory_system_init(void) {
    uint32_t a=RAM16(0x00100c22u)&1023u;
    SET_FIELD(0x40002014u, 1023u, a);
    a=RAM16(0x00100c08u)&2047u;
    SET_FIELD(0x4000200cu, 2047u, a);
    initialize_port_masks();
    system_key_sequence();
    RAM32(0x40002000u)|=0x700u;
    RAM32(0x4000200cu)=RAM16(0x00100c08u);
    RAM32(0x4000200cu)=RAM16(0x00100c06u);
    RAM32(0x4000200cu)=RAM16(0x00100c04u);
    RAM32(0x4000200cu)=RAM16(0x00100c00u);
    system_key_sequence();
    RAM32(0x40002000u)&=~0x700u;
    flash_config_mode(1);
    uint8_t cfg[4]={3, 4, 3, 2};
    configure_clock_source((uint32_t)(uintptr_t)cfg);
    system_field15(7);
    clock_select_wait(4, 1);
    set_clock_divider(4);
}
