#include "factory_layout.h"
extern void factory_bit_write(uint32_t, uint32_t, uint32_t);
uint32_t gpio_config_pin(uint32_t port, uint32_t pin, uint32_t cfg) {
    uint32_t base=0x40020d0cu+port;
    factory_bit_write(base, pin, 0);
    RAM32(0x40020c00u+port+4u*pin)=0;
    factory_bit_write(base-12, pin, RAM8(cfg));
    factory_bit_write(base+16, pin, RAM8(cfg+1));
    factory_bit_write(base+20, pin, RAM8(cfg+2));
    factory_bit_write(base+24, pin, RAM8(cfg+3));
    factory_bit_write(base+32, pin, RAM8(cfg+4));
    RAM32(0x40020f08u)=(RAM32(0x40020f08u)&~0x8000u)|((RAM8(cfg+5)&1u)<<15);
    return 0;
}
uint32_t spi_config_port(uint32_t addr, uint32_t cfg) {
    RAM32(addr)=0;
    RAM32(addr)=(uint32_t)(RAM8(cfg)|RAM8(cfg+1)|RAM8(cfg+2)|RAM8(cfg+3)|0x40u);
    RAM32(addr+8)=0;
    return 0;
}
