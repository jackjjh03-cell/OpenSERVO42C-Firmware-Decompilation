#ifndef SERVO42C_FLASH_LAYOUT_H
#define SERVO42C_FLASH_LAYOUT_H
/* HC32L130F8UA main flash layout. Reserve all device identity, settings and
 * calibration pages in application builds. Update linker and storage code
 * together if this map changes. */
#define FLASH_CAPACITY_BYTES       0x10000u
#define FLASH_PAGE_BYTES           0x0200u
#define APP_FLASH_LIMIT            0x7A00u
#define IDENTITY_PAGE              0x7A00u
#define IDENTITY_VALUE             0x7BFCu
#define SETTINGS_PAGE              0x7C00u
#define SETTINGS_RECORD_BYTES      64u
#define SETTINGS_RECORD_COUNT      8u
#define SETTINGS_LAST_RECORD       (SETTINGS_PAGE + (SETTINGS_RECORD_COUNT-1u)*SETTINGS_RECORD_BYTES)
#define CALIBRATION_KNOT_PAGE      0x7E00u
#define CALIBRATION_KNOT_META      0x7F90u
#define CALIBRATION_KNOT_VALID     0x7F94u
#define CALIBRATION_LUT_VALID      0x7F96u
#define FACTORY_ENROLLMENT_MARKER   0x7FFCu
#define CALIBRATION_LUT_BASE       0x8000u
#define CALIBRATION_LUT_ENTRIES    16384u
_Static_assert(IDENTITY_PAGE==APP_FLASH_LIMIT,"application boundary");
_Static_assert(IDENTITY_PAGE+FLASH_PAGE_BYTES==SETTINGS_PAGE,"identity page");
_Static_assert(SETTINGS_RECORD_BYTES*SETTINGS_RECORD_COUNT==FLASH_PAGE_BYTES,"settings page");
_Static_assert(SETTINGS_PAGE+FLASH_PAGE_BYTES==CALIBRATION_KNOT_PAGE,"settings boundary");
_Static_assert(CALIBRATION_KNOT_PAGE+FLASH_PAGE_BYTES==CALIBRATION_LUT_BASE,"knot boundary");
_Static_assert(CALIBRATION_LUT_BASE+2u*CALIBRATION_LUT_ENTRIES==FLASH_CAPACITY_BYTES,"calibration boundary");
#endif
