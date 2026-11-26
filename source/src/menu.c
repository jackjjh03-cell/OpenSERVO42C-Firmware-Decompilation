#include "firmware_api.h"
#define QUERY 255u
/* All callbacks preserve the legacy 0xFF query convention and commit only on change. */
uint32_t menu_autosdd(uint32_t v) {
    if (v==QUERY) return LIVE->autosdd!=0;
    if (LIVE->autosdd!=v) {
        copy_runtime_settings();
        RECORD->runtime.autosdd=(uint8_t)v;
        factory_commit();
    }
    return v;
}
uint32_t menu_interpolation(uint32_t v) {
    if (v==QUERY) return LIVE->interpolation!=0;
    if (LIVE->interpolation!=v) {
        copy_runtime_settings();
        RECORD->runtime.interpolation=(uint8_t)v;
        factory_commit();
    }
    return v;
}
uint32_t menu_direction(uint32_t v) {
    if (v==QUERY) return LIVE->direction;
    if (LIVE->direction!=v) {
        copy_runtime_settings();
        RAM8(WORK_RAM(WS_SETTINGS_CHANGE_REASON))=2;
        RECORD->runtime.direction=(uint8_t)v;
        factory_commit();
    }
    return v;
}
uint32_t menu_enable(uint32_t v) {
    if (v==QUERY) return LIVE->enable_mode;
    if (LIVE->enable_mode!=v) {
        copy_runtime_settings();
        RECORD->runtime.enable_mode=(uint8_t)v;
        factory_commit();
    }
    return v;
}
void menu_exit(void) {
    RAM8(WORK_RAM(WS_MENU_STATE))=0;
    RAM8(WORK_RAM(WS_MENU_REDRAW_FLAG))=0;
}
uint32_t menu_uart_address(uint32_t v) {
    if (v==QUERY) return (uint32_t)LIVE->uart_address-0xE0u;
    if (LIVE->uart_address!=v+0xE0u) {
        copy_runtime_settings();
        RECORD->runtime.uart_address=(uint16_t)(v+0xE0u);
        factory_commit();
    }
    return v;
}
uint32_t menu_origin_direction(uint32_t v) {
    if (v==QUERY) return ORIGIN->direction;
    if (ORIGIN->direction!=v) {
        copy_runtime_settings();
        RECORD->origin.direction=(uint8_t)v;
        factory_commit();
    }
    return v;
}
uint32_t menu_origin_speed(uint32_t v) {
    if (v==QUERY) return ORIGIN->speed;
    if (ORIGIN->speed!=v) {
        copy_runtime_settings();
        RECORD->origin.speed=(uint16_t)v;
        factory_commit();
    }
    return v;
}
uint32_t menu_origin_mode(uint32_t v) {
    if (v==QUERY) return ORIGIN->mode;
    if (ORIGIN->mode!=v) {
        copy_runtime_settings();
        if (v==0) RECORD->origin.position=0xFFFF;
        RECORD->origin.mode=(uint8_t)v;
        factory_commit();
    }
    return v;
}
uint32_t menu_motor_type(uint32_t v) {
    if (v!=QUERY) {
        uint16_t geometry=v==0?50:25;
        if (MOTOR->motor_geometry!=geometry) {
            copy_runtime_settings();
            RECORD->motor.motor_geometry=geometry;
            factory_commit();
        }
    }
    return MOTOR->motor_geometry!=50;
}
uint32_t menu_microsteps(uint32_t v) {
    if (v==QUERY) {
        uint16_t m=LIVE->microsteps;
        for (uint32_t i=0;i<9;i++) if (m==(1u<<i))return i;
        return 9;
    }
    if (v==9) {
        if (LIVE->microsteps!=LIVE->alternate_microsteps && LIVE->alternate_microsteps!=0) {
            copy_runtime_settings();
            RECORD->runtime.microsteps=LIVE->alternate_microsteps;
            factory_commit();
        }
    }
    else {
        uint32_t shift=v&255u;
        uint32_t m=shift<32?1u<<shift:0;
        if (LIVE->microsteps!=m) {
            copy_runtime_settings();
            RECORD->runtime.microsteps=(uint16_t)m;
            factory_commit();
        }
    }
    return v;
}
uint32_t menu_protection(uint32_t v) {
    if (v==QUERY) return LIVE->protection!=0;
    if (LIVE->protection!=v) {
        copy_runtime_settings();
        RAM8(WORK_RAM(WS_SETTINGS_CHANGE_REASON))=4;
        RECORD->runtime.protection=(uint8_t)v;
        factory_commit();
    }
    return v;
}
uint32_t menu_control_mode(uint32_t v) {
    if (v==QUERY) return LIVE->control_mode;
    if (LIVE->control_mode!=v) {
        copy_runtime_settings();
        RAM8(WORK_RAM(WS_SETTINGS_CHANGE_REASON))=1;
        RECORD->runtime.control_mode=(uint8_t)v;
        factory_commit();
    }
    if (LIVE->control_mode==2) {
        RAM8(WORK_RAM(WS_UART_ENABLE_REQUEST))=1;
        factory_refresh_mode();
    }
    return v;
}
/* preserve the factory mapping, including the index-0 fallback. */
uint32_t menu_acceleration(uint32_t v) {
    if (v==QUERY)return LIVE->acceleration_index;
    if (LIVE->acceleration_index!=v) {
        copy_runtime_settings();
        RECORD->runtime.acceleration_index=(uint16_t)v;
        uint16_t index=RECORD->runtime.acceleration_index;
        RECORD->extra.acceleration=(index>=1 && index<=7)?(uint16_t)(286u+(index-1u)*126u):286u;
        factory_commit();
    }
    return v;
}
/* odd but deliberate compatibility: selecting 0 (or out-of-range)
 * actually uses 38400, despite the menu text reading Disable. Query of zero
 * returns index 0. Unrecognized query values clear the live baud word. */
uint32_t menu_uart_baud(uint32_t v) {
    if (v==QUERY) {
        uint32_t b=LIVE->uart_baud;
        switch (b) {
        case 0:return 0;
        case 9600:return 1;
        case 19200:return 2;
        case 25000:return 3;
        case 38400:return 4;
        case 57600:return 5;
        case 115200:return 6;
        default:LIVE->uart_baud=0;
            return 0;
        }
    }
    uint32_t b=38400;
    switch (v) {
    case 1:b=9600;
        break;
    case 2:b=19200;
        break;
    case 3:b=25000;
        break;
    case 5:b=57600;
        break;
    case 6:b=115200;
        break;
    default:break;
    }
    if (LIVE->uart_baud!=b) {
        copy_runtime_settings();
        RAM8(WORK_RAM(WS_SETTINGS_CHANGE_REASON))=3;
        RECORD->runtime.uart_baud=b;
        factory_commit();
    }
    return v;
}
/* keep the existing compiler division helper for this build. */
extern uint32_t factory_divide(uint32_t numerator, uint32_t denominator);
uint32_t menu_current(uint32_t v) {
    if (v==QUERY)return factory_divide(MOTOR->current_raw, 200);
    uint32_t current=v*200u;
    if (MOTOR->current_raw!=current) {
        copy_runtime_settings();
        RECORD->motor.current_raw=(uint16_t)current;
        RECORD->motor.half_current_raw=(uint16_t)(v*100u);
        factory_commit();
    }
    return v;
}
