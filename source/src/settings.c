#include "firmware_api.h"
#include "defaults.h"
/* deliberately preserve all padding/reserved bytes. */
__attribute__((noinline)) void copy_runtime_settings(void) {
    RECORD->runtime.microsteps=LIVE->microsteps;
    RECORD->runtime.alternate_microsteps=LIVE->alternate_microsteps;
    RECORD->runtime.direction=LIVE->direction;
    RECORD->runtime.error_limit=LIVE->error_limit;
    RECORD->runtime.enable_mode=LIVE->enable_mode;
    RECORD->runtime.control_mode=LIVE->control_mode;
    RECORD->runtime.autosdd=LIVE->autosdd;
    RECORD->runtime.protection=LIVE->protection;
    RECORD->runtime.interpolation=LIVE->interpolation;
    RECORD->runtime.uart_baud=LIVE->uart_baud;
    RECORD->runtime.uart_address=LIVE->uart_address;
    RECORD->runtime.motion_saved_raw=LIVE->motion_saved_raw;
    RECORD->runtime.acceleration_index=LIVE->acceleration_index;
    RECORD->runtime.torque_raw=LIVE->torque_raw;
    RECORD->motor.current_raw=MOTOR->current_raw;
    RECORD->motor.half_current_raw=MOTOR->half_current_raw;
    RECORD->motor.motor_geometry=MOTOR->motor_geometry;
    RECORD->origin.mode=ORIGIN->mode;
    RECORD->origin.position=ORIGIN->position;
    RECORD->origin.speed=ORIGIN->speed;
    RECORD->origin.direction=ORIGIN->direction;
    RECORD->gains.kp=GAINS->kp;
    RECORD->gains.ki=GAINS->ki;
    RECORD->gains.kd=GAINS->kd;
    RECORD->extra.acceleration=EXTRA->acceleration;
    RECORD->extra.unknown32=EXTRA->unknown32;
    RECORD->extra.unknown34=EXTRA->unknown34;
    RECORD->valid=1;
}
/* Copy only fields present in the original load routine. */
__attribute__((noinline)) void read_active_settings_record(void) {
    volatile const SettingsRecord *s=PTR(SettingsRecord, RAM32(WORK_RAM(WS_ACTIVE_SETTINGS_ADDRESS)));
#define LOAD(field) LIVE->field=s->runtime.field
    LOAD(microsteps);
    LOAD(alternate_microsteps);
    LOAD(direction);
    LOAD(error_limit);
    LOAD(enable_mode);
    LOAD(control_mode);
    LOAD(autosdd);
    LOAD(protection);
    LOAD(interpolation);
    LOAD(uart_baud);
    LOAD(uart_address);
    LOAD(motion_saved_raw);
    LOAD(acceleration_index);
    LOAD(torque_raw);
#undef LOAD
    MOTOR->current_raw=s->motor.current_raw;
    MOTOR->half_current_raw=s->motor.half_current_raw;
    MOTOR->motor_geometry=s->motor.motor_geometry;
    ORIGIN->mode=s->origin.mode;
    ORIGIN->position=s->origin.position;
    ORIGIN->speed=s->origin.speed;
    ORIGIN->direction=s->origin.direction;
    GAINS->kp=s->gains.kp;
    GAINS->ki=s->gains.ki;
    GAINS->kd=s->gains.kd;
    EXTRA->acceleration=s->extra.acceleration;
    EXTRA->unknown32=s->extra.unknown32;
    EXTRA->unknown34=s->extra.unknown34;
}
/* scan backward, testing the first halfword (not the valid flag). */
void select_settings_record(void) {
    RAM32(WORK_RAM(WS_ACTIVE_SETTINGS_ADDRESS))=SETTINGS_PAGE;
    for (unsigned i=7;i>0;i--) {
        uint32_t a=SETTINGS_PAGE+64*i;
        if (RAM16(a)!=0xFFFF) {
            RAM32(WORK_RAM(WS_ACTIVE_SETTINGS_ADDRESS))=a;
            return;
        }
    }
}
/* original blank check examines 22 halfwords, not all 64 bytes. */
__attribute__((noinline)) uint32_t settings_slot_blank(void) {
    for (unsigned i=0;i<22;i++)if (RAM16(RAM32(WORK_RAM(WS_ACTIVE_SETTINGS_ADDRESS))+2*i)!=0xFFFF)return 0;
    return 1;
}
/* defaults from the binary, NOT necessarily the live user configuration. */
void load_or_initialize_settings(void) {
    select_settings_record();
    if (RAM16(RAM32(WORK_RAM(WS_ACTIVE_SETTINGS_ADDRESS))+54)==1) {
        read_active_settings_record();
    }
    else {
        GAINS->kp=DEFAULT_KP;
        GAINS->ki=DEFAULT_KI;
        GAINS->kd=DEFAULT_KD;
        LIVE->microsteps=DEFAULT_MICROSTEPS;
        LIVE->alternate_microsteps=DEFAULT_ALTERNATE_MICROSTEPS;
        LIVE->direction=DEFAULT_DIRECTION;
        LIVE->error_limit=DEFAULT_ERROR_LIMIT_RAW;
        LIVE->enable_mode=DEFAULT_ENABLE_MODE;
        LIVE->control_mode=DEFAULT_CONTROL_MODE;
        LIVE->autosdd=DEFAULT_AUTOSDD;
        LIVE->protection=DEFAULT_PROTECTION;
        LIVE->interpolation=DEFAULT_INTERPOLATION;
        LIVE->uart_baud=DEFAULT_UART_BAUD_RAW;
        LIVE->uart_address=DEFAULT_UART_ADDRESS;
        LIVE->motion_saved_raw=DEFAULT_MOTION_SAVED_RAW;
        LIVE->acceleration_index=DEFAULT_ACCELERATION_INDEX;
        LIVE->torque_raw=DEFAULT_TORQUE_RAW;
        EXTRA->acceleration=DEFAULT_ACCELERATION_RAW;
        EXTRA->unknown32=2;
        EXTRA->unknown34=1;
        ORIGIN->mode=DEFAULT_ORIGIN_MODE;
        ORIGIN->position=DEFAULT_ORIGIN_POSITION;
        ORIGIN->speed=DEFAULT_ORIGIN_SPEED_INDEX;
        ORIGIN->direction=DEFAULT_ORIGIN_DIRECTION;
        MOTOR->motor_geometry=DEFAULT_MOTOR_GEOMETRY;
        MOTOR->half_current_raw=DEFAULT_HALF_CURRENT_RAW;
        MOTOR->current_raw=DEFAULT_CURRENT_RAW;
        RAM16(WORK_RAM(0x0650u))=1;
    }
    copy_runtime_settings();
}

/* Persistent settings transaction. Preserve record padding, change-reason
 * fields and flash operation order. */
extern void factory_pause_control(void), factory_resume_control(void);
extern void factory_flash_erase_write(uint32_t, uint32_t, uint32_t), factory_flash_write(uint32_t, uint32_t, uint32_t);
extern uint32_t factory_divide(uint32_t, uint32_t), factory_apply_mode(uint32_t);
extern void factory_format_uint(uint32_t, uint32_t), factory_apply_direction(void), factory_apply_uart(void);
void factory_commit(void) {
 #define ACTIVE_RECORD RAM32(WORK_RAM(WS_ACTIVE_SETTINGS_ADDRESS))
 #define CHANGE_REASON RAM8(WORK_RAM(WS_SETTINGS_CHANGE_REASON))
 #define GATE RAM32(0x4000580C)
 #define CONTROL_STATE RAM8(WORK_RAM(WS_MOTION_FEEDBACK))
    RECORD->valid=1;
    if (RAM16(ACTIVE_RECORD+54)==1 && ACTIVE_RECORD+64u<=SETTINGS_LAST_RECORD) {
        ACTIVE_RECORD+=64;
        while (!settings_slot_blank()) {
            if (ACTIVE_RECORD+64u>SETTINGS_LAST_RECORD) {
                ACTIVE_RECORD=SETTINGS_PAGE;
                break;
            }
            ACTIVE_RECORD+=64;
        }
    }else ACTIVE_RECORD=SETTINGS_PAGE;
    factory_pause_control();
    if (ACTIVE_RECORD==SETTINGS_PAGE)factory_flash_erase_write(ACTIVE_RECORD, WORK_RAM(WS_PENDING_SETTINGS_RECORD), 28);
    else factory_flash_write(ACTIVE_RECORD, WORK_RAM(WS_PENDING_SETTINGS_RECORD), 28);
    read_active_settings_record();
    copy_runtime_settings();
    RAM32(WORK_RAM(WS_POSITIVE_LIMIT))=factory_divide(8192, MOTOR->motor_geometry);
    RAM32(WORK_RAM(WS_NEGATIVE_LIMIT))=0u-RAM32(WORK_RAM(WS_POSITIVE_LIMIT));
    RAM32(WORK_RAM(WS_PULSE_SCALE))=factory_divide((LIVE->microsteps>>1)+8192, LIVE->microsteps);
    uint32_t numerator=(uint32_t)(((int32_t)((uint32_t)LIVE->microsteps*MOTOR->motor_geometry))>>1)+0x200000u;
    RAM32(WORK_RAM(WS_INTERPOLATION_NUMERATOR))=factory_divide(numerator, (uint32_t)LIVE->microsteps*MOTOR->motor_geometry);
    factory_format_uint(LIVE->alternate_microsteps, WORK_RAM(0x0145u));
    if (CHANGE_REASON==1 && (GATE&1u)) {
        CONTROL_STATE=(uint8_t)factory_apply_mode(LIVE->control_mode);
        RAM8(WORK_RAM(WS_STALL_FLAG))=0;
    }
    uint8_t reason=CHANGE_REASON;
    if (reason==4)RAM32(WORK_RAM(WS_MENU_TABLE_ADDRESS))=WORK_RAM(0x0358u);
    if (reason==2)factory_apply_direction();
    if (LIVE->control_mode==2)RAM16(WORK_RAM(0x061au))=(uint16_t)factory_divide(RAM16(WORK_RAM(WS_UART_MOTION_SPEED)), LIVE->microsteps);
    if (CHANGE_REASON==3)factory_apply_uart();
    CHANGE_REASON=0;
    if ((GATE&1u)&&CONTROL_STATE==0)factory_resume_control();
 #undef ACTIVE_RECORD
 #undef CHANGE_REASON
 #undef GATE
 #undef CONTROL_STATE
}
