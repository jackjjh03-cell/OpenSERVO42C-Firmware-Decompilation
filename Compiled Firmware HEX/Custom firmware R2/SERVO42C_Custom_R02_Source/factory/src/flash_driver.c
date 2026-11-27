#include "factory_layout.h"
/* Factory flash-controller operations. Keep original sequence and error codes.
 * The controller's actual erase/program behavior needs hardware validation.
 * No flash write is performed by the host tests. */
#define FLASH_CONTROL RAM32(0x40020020u)
#define FLASH_KEY RAM32(0x4002002cu)
#define FLASH_FLAGS RAM32(0x40020030u)
static inline __attribute__((always_inline)) void key(void) {
    FLASH_KEY=0x5a5au;
    FLASH_KEY=0xa5a5u;
}
void flash_clear_flags(void) {
    key();
    FLASH_FLAGS=0xffffffffu;
}
uint32_t flash_config_mode(uint32_t mode) {
    key();
    FLASH_CONTROL=(FLASH_CONTROL&~12u)|((mode&3u)<<2);
    return 0;
}
static inline __attribute__((always_inline)) int wait_idle(void) {
    for (unsigned i=0;i<256;++i)if (!(FLASH_CONTROL&16u))return 1;
    return 0;
}
static inline __attribute__((always_inline)) int set_operation(uint32_t mode) {
    for (unsigned i=0;i<255;++i) {
        if ((FLASH_CONTROL&3u)==mode)return 1;
        key();
        FLASH_CONTROL=(FLASH_CONTROL&~3u)|mode;
    }
    return (FLASH_CONTROL&3u)==mode;
}
extern void flash_key_sequence(void);
/* At 0x1188 the factory erase initiator stores one zero byte to the target. */
uint32_t flash_erase_page(uint32_t address) {
    if (address>=FLASH_CAPACITY_BYTES)return 4;
    if (!wait_idle())return 9;
    flash_clear_flags();
    if (!set_operation(2))return 9;
    RAM8(address)=0;
    if (!wait_idle())return 9;
    flash_key_sequence();
    return 0;
}
uint32_t flash_program_halfword(uint32_t address, uint32_t value) {
    if (address>=FLASH_CAPACITY_BYTES)return 4;
    if (!wait_idle())return 9;
    if (!set_operation(1))return 9;
    flash_clear_flags();
    RAM16(address)=(uint16_t)value;
    if (!wait_idle())return 9;
    flash_key_sequence();
    return 0;
}
