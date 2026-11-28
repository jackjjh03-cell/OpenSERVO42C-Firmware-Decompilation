#ifndef OS_SETTINGS_SESSION_H
#define OS_SETTINGS_SESSION_H
#include "run.h"
#include "settings.h"
#include "protocol.h"
enum { OS_GET_STORAGE=26, OS_SAVE_SETTINGS=27, OS_GET_DRIVE=28, OS_SET_DRIVE=29 };
/* Flash validity is not hardware commissioning or a mechanical reference. */
typedef struct { uint32_t generation; bool loaded; uint8_t enable_high; } os_settings_session;
bool os_settings_boot(os_settings_session *session,os_run *r,const os_board_io *board,const os_settings_io *store,const os_settings *fallback,const os_offsets *offsets,bool hardware_validated);
/* Caller has stopped ADC/position scheduling, prevented rearming, and prepared flash timing. */
bool os_settings_commit(os_settings_session *session,os_run *r,const os_settings_io *store,bool quiesced);
/* Call only for storage opcodes. Nonzero quiesced requires target-controlled maintenance. */
size_t os_storage_dispatch(os_settings_session *session,os_run *r,const os_settings_io *store,bool quiesced,const uint8_t *in,size_t n,uint8_t *out);
#endif
