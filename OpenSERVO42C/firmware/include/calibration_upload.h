#ifndef OS_CALIBRATION_UPLOAD_H
#define OS_CALIBRATION_UPLOAD_H
#include "settings_session.h"
#include "settings_flash.h"
#include "calibration_check.h"
enum {OS_LUT_BEGIN=44,OS_LUT_WRITE=45,OS_LUT_FINISH=46,OS_LUT_STATUS=47,OS_LUT_ABORT=48};
typedef struct {uint32_t crc;uint16_t next;bool active;} os_lut_upload;
/* Serialized, with output off, EN inactive and flash scheduling quiesced.
 * LUT is the same mapped memory that the flash callbacks modify. */
size_t os_lut_dispatch(os_lut_upload *u,os_settings_session *session,os_flash *flash,const os_settings_io *store,
                       os_calibration_check *check,const uint16_t *lut,bool quiesced,const uint8_t *in,size_t n,uint8_t *out);
#endif
