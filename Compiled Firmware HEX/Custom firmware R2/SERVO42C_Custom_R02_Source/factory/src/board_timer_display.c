#include "factory_layout.h"
/* Initialize GPIO configuration bytes explicitly. These values apply to the
 * reset-to-board-initialization sequence; preserve the pin setup and
 * register access order. */
#define ADDRESS(p) ((uint32_t)(uintptr_t)(p))
extern uint32_t gpio_config_pin(uint32_t, uint32_t, uint32_t), gpio_set_direction_bit(uint32_t, uint32_t), gpio_clear_output_bit(uint32_t, uint32_t), gpio_write_pin_register(uint32_t, uint32_t, uint32_t);
extern uint32_t timer_config_extended(uint32_t, uint32_t), timer_config_capture(uint32_t, uint32_t), timer_config_18(uint32_t, uint32_t), timer_config_basic(uint32_t, uint32_t), timer_write_count_mode(uint32_t, uint32_t, uint32_t), timer_write_compare(uint32_t, uint32_t, uint32_t), timer_write_34(uint32_t, uint32_t), timer_reload_alias(uint32_t, uint32_t), timer_config_30(uint32_t, uint32_t, uint32_t), timer_write_count(uint32_t, uint32_t), factory_timer_reload(uint32_t, uint32_t), timer_clear_flag(uint32_t, uint32_t), timer_set_400(uint32_t);
extern uint32_t peripheral_clock_frequency(void), i2c_configure(uint32_t, uint32_t), reg_set_bit_plus8(uint32_t, uint32_t);
extern void factory_irq_config(uint32_t, uint32_t, uint32_t);
void initialize_motor_timers(void) {
    uint8_t pin_cfg[6]={0, 0, 0, 0, 0, 1};
    gpio_set_direction_bit(0, 15);
    gpio_config_pin(0, 15, ADDRESS(pin_cfg));
    gpio_write_pin_register(0, 15, 5);
    gpio_set_direction_bit(64, 3);
    gpio_config_pin(64, 3, ADDRESS(pin_cfg));
    gpio_write_pin_register(64, 3, 2);
    gpio_set_direction_bit(64, 6);
    gpio_config_pin(64, 6, ADDRESS(pin_cfg));
    gpio_write_pin_register(64, 6, 3);
    gpio_set_direction_bit(0, 2);
    gpio_config_pin(0, 2, ADDRESS(pin_cfg));
    gpio_write_pin_register(0, 2, 4);
    uint8_t basic[8]={3, 0, 0, 0, 0, 1, 0, 0};
    uint8_t capture[10]={0, 7, 0, 1, 0, 0, 7, 0, 1, 0};
    for (uint32_t t=0;t<2;t++) {
        timer_config_extended(t, ADDRESS(basic));
        timer_write_count_mode(t, 1200, 1);
        timer_write_compare(t, 0, 1200);
        timer_write_compare(t, 1, 1200);
        timer_config_capture(t, ADDRESS(capture));
        timer_write_34(t, 1);
        timer_reload_alias(t, 0);
        timer_config_30(t, 1, 0);
    }
    uint8_t a[4]={1, 1, 0, 0};
    timer_config_18(0, ADDRESS(a));
    a[0]=0;
    a[1]=0;
    a[2]=2;
    a[3]=1;
    timer_config_18(1, ADDRESS(a));
}
void initialize_display_bus(void) {
    uint8_t pin_cfg[6]={0, 0, 0, 0, 0, 1};
    gpio_clear_output_bit(0, 12);
    gpio_config_pin(0, 12, ADDRESS(pin_cfg));
    pin_cfg[2]=1;
    pin_cfg[4]=1;
    gpio_config_pin(192, 0, ADDRESS(pin_cfg));
    gpio_config_pin(192, 1, ADDRESS(pin_cfg));
    gpio_write_pin_register(192, 0, 1);
    gpio_write_pin_register(192, 1, 1);
    struct {
        uint32_t clock, baud;
        uint8_t mode, address, low_bit, pad;
    }
    bus;
    bus.clock=peripheral_clock_frequency();
    bus.baud=400000;
    bus.mode=0x40;
    bus.address=0x55;
    bus.low_bit=0;
    bus.pad=0;
    i2c_configure(0x40000400, ADDRESS(&bus));
    reg_set_bit_plus8(0x40000400, 6);
    uint8_t timer[7]={0, 0, 0, 7, 0, 0, 1};
    timer_config_basic(2, ADDRESS(timer));
    timer_write_count(2, 0xfe89);
    factory_timer_reload(2, 0xfe89);
    timer_clear_flag(2, 0);
    timer_set_400(2);
    factory_irq_config(16, 1, 1);
}
