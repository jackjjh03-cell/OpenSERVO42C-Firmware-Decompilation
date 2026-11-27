#include "factory_layout.h"
/* NVIC pending, priority, enable/disable and synchronization. */
__attribute__((noinline)) void factory_irq_config(uint32_t irq, uint32_t priority, uint32_t enable) {
    uint32_t bit=1u<<(irq&31), shift=(irq&3u)*8u;
    if ((int32_t)irq>=0)RAM32(0xe000e280u)=bit;
    uint32_t addr=(int32_t)irq>=0?0xe000e400u+(irq&~3u):0xe000ed1cu+(((irq&15u)-8u)&~3u);
    RAM32(addr)=(RAM32(addr)&~(255u<<shift))|((priority&3u)<<(shift+6));
    if ((int32_t)irq>=0) {
        if (enable==1)RAM32(0xe000e100u)=bit;
        else {
            RAM32(0xe000e180u)=bit;
            __asm__ volatile("dsb sy\n\tisb sy":::"memory");
        }
    }
}
/* flash timing coefficients extracted at 0x21A4, not code.
 * Original retry limit and keyed register writes retained. */
uint32_t flash_timing_setup(uint32_t scale, uint32_t flag) {
    if (scale!=1&&scale!=2&&scale!=4&&scale!=6&&scale!=8&&scale!=12)return 4;
    const uint32_t base=0x40020000u;
    RAM32(base+0x20)=(RAM32(base+0x20)&~0x200u)|((flag&1u)<<9);
    for (unsigned i=0;i<8;i++) {
        uint32_t want=(scale==12&&i==1)?255u:scale*RAM32(ASSET_ADDR(flash_timing_coefficients, 0x0u)+4*i);
        unsigned retry=255;
        while (RAM32(base+4*i)!=want) {
            if (!retry--)return 7;
            RAM32(base+0x2c)=0x5a5a;
            RAM32(base+0x2c)=0xa5a5;
            RAM32(base+4*i)=want;
        }
    }
    return 0;
}
extern uint32_t factory_divide(uint32_t, uint32_t), reg_set_bit_plus4(uint32_t, uint32_t);
/* mode fields and baud divider. Factory uses a two-entry prescale
 * table (4,3). Inputs with other prescale indices are not valid configs. */
uint32_t uart_configure(uint32_t base, uint32_t cfg) {
    if (!cfg)return 4;
    RAM32(base+4)=0;
    RAM32(base+4)=RAM16(cfg+2)|RAM8(cfg+1)|RAM8(cfg)|RAM16(cfg+4);
    uint8_t mode=RAM8(cfg);
    if (mode==0x40||mode==0xc0) {
        uint32_t index=RAM16(cfg+4)>>9;
  /* Preserve actual factory table reads for valid inputs. */
        uint32_t shift=RAM8(ASSET_ADDR(uart_prescale_words, 0x0u)+index*4u);
        uint32_t baud=RAM32(cfg+12);
        uint32_t denominator=shift<32?baud<<shift:0;
        RAM32(base+0x18)=factory_divide(RAM32(cfg+8), denominator);
        reg_set_bit_plus4(base, 4);
    }
    return 0;
}
uint32_t factory_strlen(uint32_t s) {
    uint32_t p=s+1;
    while (RAM8(s))++s;
    return s+1-p;
}
extern uint32_t timer2_configure(uint32_t), timer2_write_count(uint32_t), timer2_write_reload(uint32_t), timer2_clear_flag(uint32_t), timer2_start(void);
void initialize_motion_tick(void) {
    volatile uint32_t config[2];
    config[0]=0x06010100u;
    config[1]=0x00010001u;
    timer2_configure((uint32_t)(uintptr_t)config);
    timer2_write_count(0xffb6);
    timer2_write_reload(0xffb6);
    timer2_clear_flag(0);
    factory_irq_config(5, 2, 1);
    timer2_start();
}
