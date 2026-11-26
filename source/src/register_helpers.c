#include "factory_layout.h"
/* Low-level register operations. Preserve volatile access order and ARM
 * shift semantics; some field meanings are not confirmed. */
void reg_write_bit(uint32_t address, uint32_t bit, uint32_t enable) {
    uint32_t mask=bit&255u;
    mask=mask<32u?1u<<mask:0u;
    if (enable==1u)RAM32(address)|=mask;
    else RAM32(address)&=~mask;
}
uint32_t reg_read_bit(uint32_t address, uint32_t bit) {
    uint32_t value=RAM32(address);
    bit&=255u;
    return bit<32u?(value>>bit)&1u:0u;
}
uint16_t read_halfword(uint32_t address) {
    return RAM16(address);
}
uint32_t read_word(uint32_t address) {
    return RAM32(address);
}
void clock_clear_field(uint32_t index) {
    RAM32(0x40021014u+4u*index)&=~0x70000u;
}
void clock_set_high_bit(void) {
    RAM32(0x40021000u)|=0x80000000u;
}
void clock_set_index_high_bit(uint32_t index) {
    RAM32(0x40021010u+4u*index)|=0x80000000u;
}
/* Separate link units retain the original ABI of the shared bit helper. */
extern void factory_bit_write(uint32_t, uint32_t, uint32_t);
extern uint32_t factory_bit_read(uint32_t, uint32_t);
uint32_t gpio_clear_irq_field(uint32_t port, uint32_t bit) {
    factory_bit_write(0x40020e10u+port, bit, 0);
    return 0;
}
uint32_t gpio_set_direction_bit(uint32_t port, uint32_t bit) {
    factory_bit_write(0x40020d14u+port, bit, 1);
    return 0;
}
uint32_t gpio_clear_indexed_field(uint32_t port, uint32_t bit, uint32_t index) {
    factory_bit_write(0x40020d30u+port+4u*index, bit, 0);
    return 0;
}
uint32_t gpio_set_indexed_field(uint32_t port, uint32_t bit, uint32_t index) {
    factory_bit_write(0x40020d30u+port+4u*index, bit, 1);
    return 0;
}
uint32_t gpio_read_input_bit(uint32_t port, uint32_t bit) {
    return factory_bit_read(0x40020d04u+port, bit);
}
uint32_t gpio_read_irq_bit(uint32_t port, uint32_t bit) {
    return factory_bit_read(0x40020e00u+port, bit);
}
uint32_t gpio_write_pin_register(uint32_t port, uint32_t pin, uint32_t value) {
    RAM32(0x40020c00u+port+4u*pin)=value;
    return 0;
}
uint32_t gpio_set_output_bit(uint32_t port, uint32_t bit) {
    factory_bit_write(0x40020d0cu+port, bit, 1);
    return 0;
}
uint32_t gpio_clear_output_bit(uint32_t port, uint32_t bit) {
    factory_bit_write(0x40020d10u+port, bit, 1);
    return 0;
}
uint32_t status_bit0(void) {
    return RAM32(0x40021814u)&1u;
}
uint32_t status_bit1(void) {
    return (RAM32(0x40021814u)>>1)&1u;
}
uint32_t reg_write_plus4(uint32_t address, uint32_t value) {
    RAM32(address+4)=value;
    return 0;
}
uint32_t reg_set_bit_plus8(uint32_t address, uint32_t bit) {
    factory_bit_write(address+8, bit, 1);
    return 0;
}
uint32_t reg_set_bit_plus4(uint32_t address, uint32_t bit) {
    factory_bit_write(address+4, bit, 1);
    return 0;
}
uint32_t sys_gate_bit(uint32_t bit, uint32_t enable) {
    factory_bit_write(0x40002020u, bit, enable);
    return 0;
}
uint32_t timer2_clear_flag(uint32_t bit) {
    bit&=255u;
    RAM32(0x40005814u)=~(bit<32u?1u<<bit:0u);
    return 0;
}
uint32_t timer2_write_count(uint32_t value) {
    RAM32(0x40005800u)=(RAM32(0x40005800u)&0xffff0000u)|value;
    return 0;
}
uint32_t timer2_write_reload(uint32_t value) {
    RAM32(0x40005804u)=(RAM32(0x40005804u)&0xffff0000u)|value;
    return 0;
}
uint32_t timer2_start(void) {
    RAM32(0x4000580cu)|=1u;
    return 0;
}
void timer2_pause_irq(void) {
    RAM32(0x4000580cu)&=~0x400u;
    RAM32(0x40005814u)=0xfeu;
}
void timer2_resume_irq(void) {
    RAM32(0x40005814u)=0xfeu;
    RAM32(0x4000580cu)|=0x400u;
}
