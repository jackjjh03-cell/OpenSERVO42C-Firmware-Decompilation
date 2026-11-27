#include "factory_layout.h"
/* original indexed peripheral configuration. */
uint32_t configure_indexed_clock_adc(uint32_t index, uint32_t cfg) {
    if (!cfg)return 4;
    uint32_t base=0x40021000u+4*index;
    RAM32(base+20)=0;
    uint32_t v=RAM32(cfg);
    for (unsigned i=8;i<=28;i+=4)v|=RAM32(cfg+i);
    v|=RAM8(cfg+40);
    RAM32(base+20)=v;
    RAM32(base+16)&=0xe0300000u;
    RAM32(base+16)|=(uint32_t)(RAM16(cfg+6)-1u)|((uint32_t)(RAM16(cfg+4)-1u)<<16)|((uint32_t)RAM8(cfg+48)<<23);
    RAM32(0x40021000u)|=RAM32(cfg+44);
    RAM32(base+24)=RAM32(cfg+32);
    RAM32(base+28)=RAM32(cfg+36);
    return 0;
}
