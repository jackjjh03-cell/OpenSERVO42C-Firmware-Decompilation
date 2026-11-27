#include "firmware_api.h"
#include "sysinfo.h"
/* UART command dispatch. Packet fields and settings are expressed in raw
 * protocol units where physical units are not established. */
extern void factory_start_reply(void), factory_restore(void), factory_stop(void);
extern uint32_t factory_calibrate(uint32_t);
extern void factory_format_uint(uint32_t, uint32_t);
extern uint32_t factory_timer_reload(uint32_t, uint32_t), factory_timer_start(uint32_t), factory_timer_stop(uint32_t);
#define CONTROL_ACTIVE RAM8(WORK_RAM(WS_STALL_FLAG))
#define MOTION_MODE RAM8(WORK_RAM(WS_UART_MOTION_STATE))
#define MOTION_DIR RAM8(WORK_RAM(WS_UART_MOTION_DIRECTION))
#define MOTION_SPEED RAM16(WORK_RAM(WS_UART_MOTION_SPEED))
#define MOTION_TARGET_LO RAM32(WORK_RAM(WS_UART_TARGET_LOW))
#define MOTION_TARGET_HI RAM32(WORK_RAM(WS_UART_TARGET_HIGH))
#define TIMER_GATE RAM32(0x4000580C)
#define TIMER_GATE_MASK 0x400u
static uint16_t be16(void) {
    return (uint16_t)((uint16_t)UART->rx[2]<<8 | UART->rx[3]);
}
static void reply(uint32_t value, unsigned n) {
    UART->tx_remaining=(uint8_t)(n+1);
    for (unsigned i=1;i<=n;i++) {
        UART->tx_reverse[i]=(uint8_t)value;
        value>>=8;
    }
    uint16_t sum=LIVE->uart_address;
    for (unsigned i=1;i<=n;i++)sum+=UART->tx_reverse[i];
    UART->tx_reverse[0]=(uint8_t)sum;
    factory_start_reply();
}
static void encoder_reply(void) {
    uint32_t hi=RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_HIGH)), lo=RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_LOW));
    for (unsigned i=1;i<=4;i++) {
        UART->tx_reverse[i]=(uint8_t)lo;
        lo>>=8;
    }
    UART->tx_reverse[5]=(uint8_t)hi;
    UART->tx_reverse[6]=(uint8_t)(hi>>8);
    uint16_t sum=LIVE->uart_address;
    for (unsigned i=1;i<=6;i++)sum+=UART->tx_reverse[i];
    UART->tx_reverse[0]=(uint8_t)sum;
    UART->tx_remaining=7;
    factory_start_reply();
}
static void set_microsteps(unsigned v) {
    unsigned steps=v?v:256u;
    unsigned index=0;
    while (index<8 && (1u<<index)!=steps)index++;
    if ((1u<<index)==steps) {
        LIVE->alternate_microsteps=0;
        RECORD->runtime.alternate_microsteps=0;
        if (LIVE->microsteps==steps)factory_format_uint(0, WORK_RAM(0x0145u));
        else menu_microsteps(index);
    }else if (LIVE->alternate_microsteps==v) {
        LIVE->microsteps=(uint16_t)v;
        RECORD->runtime.microsteps=(uint16_t)v;
    }
    else {
        LIVE->alternate_microsteps=(uint16_t)v;
        RECORD->runtime.alternate_microsteps=(uint16_t)v;
        menu_microsteps(9);
    }
}
static void motion_start(void) {
    factory_timer_reload(2, 0xFE89);
    factory_timer_start(2);
}
static void motion_command(unsigned op, unsigned v) {
    TIMER_GATE &= ~TIMER_GATE_MASK;
    MOTION_SPEED=(uint16_t)(v&127u);
    MOTION_DIR=(uint8_t)(v>>7);
    if (op==0xF6) {
        if (MOTION_MODE!=1) {
            RAM32(WORK_RAM(0x061cu))=0;
            MOTION_MODE=1;
            MOTION_TARGET_LO=0;
            MOTION_TARGET_HI=0;
            RAM16(WORK_RAM(WS_UART_PULSE_COUNTER))=RAM16(WORK_RAM(WS_PREVIOUS_PULSE_COUNT));
            motion_start();
        }
    }else {
        uint32_t count=((uint32_t)UART->rx[3]<<24)|((uint32_t)UART->rx[4]<<16)|((uint32_t)UART->rx[5]<<8)|UART->rx[6];
        MOTION_MODE=2;
        RAM16(WORK_RAM(WS_UART_PULSE_COUNTER))=RAM16(WORK_RAM(WS_PREVIOUS_PULSE_COUNT));
        uint32_t lo=RAM32(WORK_RAM(WS_MOTION_POSITION)), hi=RAM32(WORK_RAM(WS_PULSE_COUNT_HIGH));
        if (MOTION_DIR) {
            uint32_t result=lo+count;
            MOTION_TARGET_LO=result;
            MOTION_TARGET_HI=hi+(result<lo);
        }
        else {
            MOTION_TARGET_LO=lo-count;
            MOTION_TARGET_HI=hi-(lo<count);
        }
        motion_start();
    }
    TIMER_GATE |= TIMER_GATE_MASK;
}
#define MENU(fn,max) do{if(v<=(max)){fn(v);result=1;}}while(0)
void uart_dispatch(void) {
    UART->tx_remaining=0;
    if (UART->rx[0]!=LIVE->uart_address) {
        uart_reset_rx();
        return;
    }
    unsigned op=UART->rx[1], v=UART->rx[2];
    uint32_t result=0;
    unsigned n=1;
    switch (op) {
    case 0x30: if (LIVE->control_mode) {
            encoder_reply();
            return;
        }
        break;
    case 0x33: result=RAM32(WORK_RAM(WS_MOTION_POSITION))-RAM32(WORK_RAM(0x0664u));
        n=4;
        break;
    case 0x36: result=RAM32(WORK_RAM(WS_FEEDBACK_POSITION_LOW))-RAM32(WORK_RAM(0x0668u));
        n=4;
        break;
    case 0x39: result=RAM32(WORK_RAM(WS_POSITION_ERROR));
        n=2;
        break;
    case 0x3A: result=RAM8(WORK_RAM(WS_DRIVE_ENABLE_STATE))?1:2;
        break;
    case 0x3D: if (CONTROL_ACTIVE) {
            RAM8(WORK_RAM(0x05d8u))=1;
            result=1;
        }
        break;
    case 0x3E: result=CONTROL_ACTIVE?1:2;
        break;
    case 0x3F: factory_restore();
        result=1;
        break;
    case 0x80: if (v==0 && LIVE->control_mode)result=factory_calibrate(1)==1?1:2;
        break;
    case 0x81: MENU(menu_motor_type, 1);
        break;
    case 0x82: MENU(menu_control_mode, 2);
        break;
    case 0x83: MENU(menu_current, 12);
        break;
    case 0x84: set_microsteps(v);
        result=1;
        break;
    case 0x85: MENU(menu_enable, 2);
        break;
    case 0x86: MENU(menu_direction, 1);
        break;
    case 0x87: MENU(menu_autosdd, 1);
        break;
    case 0x88: MENU(menu_protection, 1);
        break;
    case 0x89: MENU(menu_interpolation, 1);
        break;
    case 0x8A: MENU(menu_uart_baud, 6);
        break;
    case 0x8B: MENU(menu_uart_address, 9);
        break;
    case 0x8C: MENU(menu_acceleration, 7);
        break;
    case 0x8D: result=sysinfo_read(v);n=4;break;
    case 0x90: MENU(menu_origin_mode, 2);
        break;
    case 0x91: if (LIVE->control_mode&&ORIGIN->mode) {
            RAM8(WORK_RAM(WS_ORIGIN_SET_PENDING))=1;
            n=0;
        }
        break;
    case 0x92: MENU(menu_origin_speed, 4);
        break;
    case 0x93: MENU(menu_origin_direction, 1);
        break;
    case 0x94: if (LIVE->control_mode&&ORIGIN->mode&&ORIGIN->position!=0xFFFF) {
            RAM8(WORK_RAM(WS_ORIGIN_RETURN_PENDING))=1;
            n=0;
        }
        break;
    case 0xA1: GAINS->kp=be16();
        RECORD->gains.kp=GAINS->kp;
        factory_commit();
        result=1;
        break;
    case 0xA2: GAINS->ki=be16();
        RECORD->gains.ki=GAINS->ki;
        factory_commit();
        result=1;
        break;
    case 0xA3: GAINS->kd=be16();
        RECORD->gains.kd=GAINS->kd;
        factory_commit();
        result=1;
        break;
    case 0xA4: EXTRA->acceleration=be16();
        RECORD->extra.acceleration=EXTRA->acceleration;
        factory_commit();
        result=1;
        break;
    case 0xA5: LIVE->torque_raw=be16();
        if (LIVE->torque_raw>1200)LIVE->torque_raw=1200;
        RECORD->runtime.torque_raw=LIVE->torque_raw;
        factory_commit();
        result=1;
        break;
    case 0xF3: if (LIVE->control_mode==2&&v<2) {
            if (RAM8(WORK_RAM(WS_UART_ENABLE_REQUEST))!=v) {
                RAM8(WORK_RAM(WS_UART_ENABLE_REQUEST))=(uint8_t)v;
                RAM8(WORK_RAM(WS_MODE_REFRESH_PENDING))=1;
            }
            result=1;
        }
        break;
    case 0xF6:case 0xFD: if (LIVE->control_mode==2) {
            motion_command(op, v);
            result=1;
        }
        break;
    case 0xF7: if (LIVE->control_mode==2) {
            factory_stop();
            result=1;
        }
        break;
    case 0xFF: if (LIVE->control_mode==2&&MOTION_MODE==1) {
            if (v==0xC8) {
                MOTION_MODE=0;
                RAM8(WORK_RAM(WS_UART_ENABLE_REQUEST))=0;
                factory_refresh_mode();
                LIVE->motion_saved_raw=(uint16_t)(MOTION_SPEED|(MOTION_DIR?0x8000u:0));
                RECORD->runtime.motion_saved_raw=LIVE->motion_saved_raw;
                factory_commit();
                result=1;
            }
            else if (v==0xCA) {
                MOTION_TARGET_LO=0;
                MOTION_TARGET_HI=0;
                RAM32(WORK_RAM(0x061cu))=0;
                MOTION_MODE=0;
                LIVE->motion_saved_raw=0xFFFF;
                RECORD->runtime.motion_saved_raw=0xFFFF;
                factory_commit();
                result=1;
                RAM16(WORK_RAM(WS_UART_PULSE_COUNTER))=RAM16(WORK_RAM(WS_PREVIOUS_PULSE_COUNT));
                MOTION_TARGET_LO=RAM32(WORK_RAM(WS_MOTION_POSITION));
                MOTION_TARGET_HI=RAM32(WORK_RAM(WS_PULSE_COUNT_HIGH));
                factory_timer_stop(2);
            }
        }
        break;
    default:break;
    }
    if (n)reply(result, n);
    else uart_reset_rx();
}
