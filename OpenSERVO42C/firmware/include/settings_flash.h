#ifndef OS_SETTINGS_FLASH_H
#define OS_SETTINGS_FLASH_H
#include "settings.h"
/* Flash transactions are foreground-only. Caller must stop motor scheduling,
 * prevent rearming and establish valid flash timing before writes. */
typedef struct {
    void *ctx;
    uint32_t (*read_reg)(void *,uint32_t);
    void (*write_reg)(void *,uint32_t,uint32_t);
    uint8_t (*read_byte)(void *,uint32_t);
    void (*erase_trigger)(void *,uint32_t);
    void (*program_halfword)(void *,uint32_t,uint16_t);
    bool write_permitted;
} os_flash;
/* Original 48 MHz flash timing table, verified by register readback. */
bool os_flash_prepare_timing(os_flash *f,uint32_t core_hz);
bool os_flash_idle(os_flash *f);
os_settings_io os_flash_settings_io(os_flash *f);
os_flash os_hc32_settings_flash(void);
#endif
