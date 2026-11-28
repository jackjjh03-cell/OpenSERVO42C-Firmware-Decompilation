#ifndef OS_SETTINGS_H
#define OS_SETTINGS_H
#include "drive.h"
#define OS_SETTINGS_SLOT0 0x7600u
#define OS_SETTINGS_SLOT1 0x7800u
#define OS_SETTINGS_PAGE 512u
#define OS_SETTINGS_BYTES 94u
typedef struct {
    os_config protection;
    os_pid_config pid;
    os_motion_config motion;
    os_limits limits;
    os_drive_config drive;
    uint8_t enable_high;
} os_settings;
typedef struct {
    void *ctx;
    bool (*read)(void *,unsigned,uint8_t *,unsigned);
    bool (*erase)(void *,unsigned);
    bool (*program)(void *,unsigned,unsigned,uint16_t);
} os_settings_io;
bool os_settings_valid(const os_settings *s);
bool os_settings_load(const os_settings_io *io,os_settings *s,uint32_t *generation,unsigned *slot);
/* Caller must ensure disabled output, safe flash timing and no concurrent access. */
bool os_settings_save(const os_settings_io *io,const os_settings *s);
#endif
