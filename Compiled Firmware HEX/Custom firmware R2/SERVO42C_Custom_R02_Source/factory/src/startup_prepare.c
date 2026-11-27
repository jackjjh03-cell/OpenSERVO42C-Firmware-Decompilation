#include "factory_layout.h"
/* Initialize feedback, step inputs and motor state. The startup offset-check
 * timer configuration uses zero for its two reserved bytes; electrical
 * timing requires bench validation. */
extern void adc_shutdown_chain(void), clock_clear_field(uint32_t), clock_set_high_bit(void), clock_set_index_high_bit(uint32_t), reset_phase_state(void), clock_adc_set_bit19(void);
extern uint32_t adc_config_sequence(uint32_t), configure_indexed_clock_adc(uint32_t, uint32_t);
extern void factory_irq_config(uint32_t, uint32_t, uint32_t);
void initialize_runtime_feedback(void) {
    adc_shutdown_chain();
    uint8_t seq[2]={3, 1};
    adc_config_sequence((uint32_t)(uintptr_t)seq);
    uint32_t cfg[13]={0, 0x00010003, 0x08000000, 0, 0, 0x00400000, 0x00200000, 0x00800000, 0x40002490, WORK_RAM(WS_ADC_DMA_SAMPLE_RAW), 1, 0, 0x24};
    configure_indexed_clock_adc(0, (uint32_t)(uintptr_t)cfg);
    clock_clear_field(0);
    factory_irq_config(4, 0, 1);
    clock_set_high_bit();
    clock_set_index_high_bit(0);
    reset_phase_state();
    clock_adc_set_bit19();
    RAM32(0x40000c0cu)|=1;
}
extern uint32_t gpio_config_pin(uint32_t, uint32_t, uint32_t), gpio_clear_irq_field(uint32_t, uint32_t), gpio_set_indexed_field(uint32_t, uint32_t, uint32_t), gpio_write_pin_register(uint32_t, uint32_t, uint32_t);
extern uint32_t register_config_50(uint32_t, uint32_t), register_write_offset4(uint32_t, uint32_t), register_write_low16_or(uint32_t, uint32_t);
extern void factory_refresh_mode(void);
void initialize_step_inputs(void) {
    uint8_t cfg[6]={1, 1, 1, 0, 0, 0};
    gpio_config_pin(64, 7, (uint32_t)(uintptr_t)cfg);
    gpio_clear_irq_field(64, 7);
    gpio_set_indexed_field(64, 7, 2);
    gpio_set_indexed_field(64, 7, 3);
    factory_irq_config(1, 3, 1);
    cfg[1]=cfg[2]=0;
    cfg[5]=1;
    gpio_config_pin(0, 11, (uint32_t)(uintptr_t)cfg);
    gpio_write_pin_register(0, 11, 7);
    gpio_config_pin(0, 8, (uint32_t)(uintptr_t)cfg);
    gpio_write_pin_register(0, 8, 6);
    uint8_t counter[3]={0, 1, 0};
    register_config_50(0x40003000u, (uint32_t)(uintptr_t)counter);
    register_write_offset4(0x40003000u, 65535);
    RAM32(0x40003088u)=0;
    RAM32(0x4000308cu)=0;
    if (LIVE->direction) {
        RAM32(0x40003088u)|=64;
        RAM32(0x4000308cu)|=16;
    }else {
        RAM32(0x40003088u)|=16;
        RAM32(0x4000308cu)|=64;
    }
    RAM32(0x4000307cu)=0;
    RAM32(0x40003068u)|=3;
    RAM32(0x40003068u)|=48;
    register_write_low16_or(0x40003000u, 0);
    RAM8(WORK_RAM(WS_UART_ENABLE_REQUEST))=1;
    factory_refresh_mode();
}
extern void load_or_initialize_settings(void), check_current_offsets(uint32_t, uint32_t), reset_position_state(void), factory_format_uint(uint32_t, uint32_t);
extern uint32_t wait_for_startup_feedback(void), encoder_startup_status(void), load_calibration_knots(void), factory_divide(uint32_t, uint32_t), factory_timer_reload(uint32_t, uint32_t), factory_timer_start(uint32_t);
extern int32_t measure_motor_geometry(void);
static int32_t geometry_abs(void) {
    uint32_t v=(uint32_t)measure_motor_geometry();
    return (int16_t)((int32_t)v<0?0u-v:v);
}
uint32_t startup_prepare(void) {
    const uint32_t state=WORK_RAM(WS_MOTION_FEEDBACK);
    if (RAM16(state+8)!=1) {
        load_or_initialize_settings();
        check_current_offsets(0, 0);
    }
    if (RAM8(state)!=1) {
        if (RAM16(RAM32(WORK_RAM(WS_ACTIVE_SETTINGS_ADDRESS))+54)!=1&&!wait_for_startup_feedback())return 4;
        initialize_runtime_feedback();
    }
    if (LIVE->control_mode&&encoder_startup_status())return 3;
    if (RAM16(RAM32(WORK_RAM(WS_ACTIVE_SETTINGS_ADDRESS))+54)!=1) {
        if (LIVE->control_mode) {
            int32_t g=geometry_abs();
            if (g<23)return 1;
            if (g<60) {
                g=geometry_abs();
                if (g<23)return 1;
                MOTOR->motor_geometry=g<60?400:200;
            }else MOTOR->motor_geometry=200;
            MOTOR->motor_geometry>>=3;
        }
        copy_runtime_settings();
        factory_commit();
    }
    if (LIVE->control_mode&&!load_calibration_knots())return 2;
    RAM32(state+16)=factory_divide(8192, MOTOR->motor_geometry);
    RAM32(state+20)=0u-RAM32(state+16);
    RAM32(state+12)=factory_divide((LIVE->microsteps>>1)+8192, LIVE->microsteps);
    uint32_t p=(uint32_t)LIVE->microsteps*MOTOR->motor_geometry;
    RAM32(state+24)=factory_divide((uint32_t)((int32_t)p>>1)+0x200000u, p);
    factory_format_uint(LIVE->alternate_microsteps, WORK_RAM(0x0145u));
    reset_position_state();
    if (LIVE->control_mode==2&&LIVE->motion_saved_raw!=65535) {
        RAM32(WORK_RAM(0x061cu))=0;
        RAM8(WORK_RAM(WS_UART_MOTION_STATE))=1;
        RAM32(WORK_RAM(WS_UART_TARGET_LOW))=0;
        RAM32(WORK_RAM(WS_UART_TARGET_HIGH))=0;
        RAM8(WORK_RAM(WS_UART_MOTION_DIRECTION))=(uint8_t)(LIVE->motion_saved_raw>>15);
        RAM16(WORK_RAM(WS_UART_MOTION_SPEED))=LIVE->motion_saved_raw&32767u;
        RAM16(WORK_RAM(0x061au))=(uint16_t)factory_divide(RAM16(WORK_RAM(WS_UART_MOTION_SPEED)), LIVE->microsteps);
        RAM16(WORK_RAM(WS_UART_PULSE_COUNTER))=RAM16(state+2);
        factory_timer_reload(2, 0xfe89);
        factory_timer_start(2);
    }
    return 0;
}
