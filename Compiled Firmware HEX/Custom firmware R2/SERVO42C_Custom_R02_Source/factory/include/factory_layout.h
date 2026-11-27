#ifndef FACTORY_LAYOUT_H
#define FACTORY_LAYOUT_H
#include <stdint.h>
#include <stddef.h>
#include "layout_assets.h"
#include "flash_layout.h"
/* Shared settings, motor, UART and workspace data layout. Fields named raw
 * or unknown do not imply physical units. */
typedef struct {
 uint16_t microsteps, alternate_microsteps;
 uint8_t direction, unknown05;
 uint16_t error_limit;
 uint8_t enable_mode, control_mode, autosdd, protection, interpolation, unknown0d[3];
 uint32_t uart_baud;
 uint16_t uart_address, motion_saved_raw, acceleration_index, torque_raw;
} RuntimeSettings;
typedef struct { uint16_t current_raw, half_current_raw, motor_geometry; } MotorSettings;
typedef struct { uint8_t mode, unknown01; uint16_t position, speed; uint8_t direction, unknown07; } OriginSettings;
typedef struct { uint16_t kp,ki,kd; } Gains;
typedef struct { uint16_t acceleration, unknown32, unknown34; } ExtraSettings;
typedef struct {
 RuntimeSettings runtime; /* tail padding, if any, is NOT part of the recovered record */
 MotorSettings motor;
 OriginSettings origin;
 Gains gains;
 ExtraSettings extra;
 uint16_t valid;
 uint8_t reserved[8];
} SettingsRecord;
_Static_assert(sizeof(RuntimeSettings)==28,"runtime size");
_Static_assert(offsetof(RuntimeSettings,uart_baud)==16,"baud");
_Static_assert(offsetof(RuntimeSettings,uart_address)==20,"address");
_Static_assert(offsetof(SettingsRecord,motor)==28,"motor");
_Static_assert(offsetof(SettingsRecord,origin)==34,"origin");
_Static_assert(offsetof(SettingsRecord,gains)==42,"gains");
_Static_assert(offsetof(SettingsRecord,extra)==48,"extra");
_Static_assert(offsetof(SettingsRecord,valid)==54,"valid");
_Static_assert(sizeof(SettingsRecord)==64,"record");
typedef struct { uint8_t count,ready,tx_remaining,rx[9],tx_reverse[8]; } UartState;
_Static_assert(offsetof(UartState,tx_reverse)==12,"UART layout");
#ifdef HOST_TEST
void *test_address(uint32_t address);
#define PTR(type,addr) ((volatile type *)test_address(addr))
#else
#define PTR(type,addr) ((volatile type *)(uintptr_t)(addr))
#endif
#define RAM8(addr) (*PTR(uint8_t,addr))
#define RAM16(addr) (*PTR(uint16_t,addr))
#define RAM32(addr) (*PTR(uint32_t,addr))
#define LIVE PTR(RuntimeSettings,WORK_RAM(WS_RUNTIME_SETTINGS))
#define RECORD PTR(SettingsRecord,WORK_RAM(WS_PENDING_SETTINGS_RECORD))
#define MOTOR PTR(MotorSettings,WORK_RAM(WS_MOTOR_SETTINGS))
#define ORIGIN PTR(OriginSettings,WORK_RAM(WS_ORIGIN_SETTINGS))
#define GAINS PTR(Gains,WORK_RAM(WS_PID_GAINS))
#define EXTRA PTR(ExtraSettings,WORK_RAM(WS_EXTRA_SETTINGS))
#define UART PTR(UartState,WORK_RAM(WS_UART_STATE))
void copy_runtime_settings(void);
/* Internal hardware and application service declarations. */
void factory_commit(void);
void factory_dispatch(void);
void factory_uart_tx_end(void);
void factory_refresh_mode(void);
#endif
