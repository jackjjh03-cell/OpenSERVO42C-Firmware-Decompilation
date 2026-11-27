#include "factory_layout.h"
/* Raw memory primitives. The factory ABI has void fill and null-safe zero. */
void fill_bytes(uint32_t address, uint32_t n, uint8_t v) {
    while (n--)RAM8(address++)=v;
}
uint32_t string_length(uint32_t address) {
    uint32_t n=0;
    while (RAM8(address+n))++n;
    return n;
}
uint32_t string_copy(uint32_t address, uint32_t src) {
    uint32_t start=address;
    uint8_t v;
    do {
        v=RAM8(src++);
        RAM8(address++)=v;
    }while (v);
    return start;
}
void zero_bytes(uint32_t address, uint32_t n) {
    if (address)while (n--)RAM8(address++)=0;
}
uint32_t abs_word(uint32_t value) {
    uint32_t mask=0u-(value>>31);
    return (value^mask)-mask;
}
