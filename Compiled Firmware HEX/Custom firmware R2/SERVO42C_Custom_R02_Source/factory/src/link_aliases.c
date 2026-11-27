#include <stdint.h>
/* Internal adapters for common hardware and application services. Some
 * interfaces intentionally discard the underlying status return; see the
 * flash service limitations. */
extern uint32_t register_start_50(uint32_t), register_stop_50(uint32_t);
void factory_aux_start(uint32_t base) {
    (void)register_start_50(base);
}
void factory_aux_stop(uint32_t base) {
    (void)register_stop_50(base);
}
extern uint32_t reg_read_bit(uint32_t, uint32_t);
uint32_t factory_bit_read(uint32_t addr, uint32_t bit) {
    return reg_read_bit(addr, bit);
}
extern void reg_write_bit(uint32_t, uint32_t, uint32_t);
void factory_bit_write(uint32_t addr, uint32_t bit, uint32_t enable) {
    reg_write_bit(addr, bit, enable);
}
extern void clock_adc_stop(void), uart_dispatch(void);
void factory_clock_adc_stop(void) {
    clock_adc_stop();
}
void factory_dispatch(void) {
    uart_dispatch();
}
extern uint32_t flash_erase_page(uint32_t), flash_program_halfword(uint32_t, uint32_t);
void factory_flash_erase(uint32_t addr) {
    (void)flash_erase_page(addr);
}
void factory_flash_halfword(uint32_t addr, uint32_t v) {
    (void)flash_program_halfword(addr, v);
}
extern uint32_t gpio_clear_output_bit(uint32_t, uint32_t), gpio_set_direction_bit(uint32_t, uint32_t);
void factory_gpio_clear(uint32_t port, uint32_t bit) {
    (void)gpio_clear_output_bit(port, bit);
}
void factory_gpio_set(uint32_t port, uint32_t bit) {
    (void)gpio_set_direction_bit(port, bit);
}
extern void pause_control_chain(void), reset_compare_values(void), uart_start_reply(void), step_input_disable(void);
void factory_pause_control(void) {
    pause_control_chain();
}
void factory_reset_compare(void) {
    reset_compare_values();
}
void factory_start_reply(void) {
    uart_start_reply();
}
void factory_step_input_disable(void) {
    step_input_disable();
}
extern uint64_t scale_position(uint64_t, uint32_t);
uint64_t factory_scaled_position(uint64_t v, uint32_t divisor) {
    return scale_position(v, divisor);
}
