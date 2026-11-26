#include "factory_layout.h"
/* Shared application and hardware service routines. */
extern void factory_delay_ms(uint32_t);
extern uint32_t gpio_read_input_bit(uint32_t, uint32_t);
extern uint32_t factory_read_encoder(void);
extern uint16_t encoder_lut_read(uint32_t);
extern uint32_t abs_word(uint32_t);
extern void factory_flash_erase_write(uint32_t, uint32_t, uint32_t);
extern void display_three_lines(uint32_t, uint32_t, uint32_t);
extern void oled_initialize(void);
extern uint32_t factory_strlen(uint32_t);
extern uint32_t string_copy(uint32_t, uint32_t);
extern void clear_feedback_state(void);
extern void fill_bytes(uint32_t, uint32_t, uint8_t);
extern void factory_aux_stop(uint32_t);
extern void factory_aux_start(uint32_t);
extern uint32_t gpio_clear_irq_field(uint32_t, uint32_t);
extern uint32_t gpio_clear_indexed_field(uint32_t, uint32_t, uint32_t);
extern uint32_t gpio_set_indexed_field(uint32_t, uint32_t, uint32_t);
extern void factory_reset_compare(void);
extern void factory_clock_adc_stop(void);
extern void factory_step_input_disable(void);
extern void timer2_pause_irq(void);
/* Exact write order matters. */
uint32_t system_field15(uint32_t v) {
    RAM32(0x4000203cu)=(RAM32(0x4000203cu)&~0x38000u)|((v&7u)<<15);
    return 0;
}
void system_key_sequence(void) {
    RAM32(0x40002008u)=0x5a5au;
    RAM32(0x40002008u)=0xa5a5u;
}
void flash_key_sequence(void) {
    RAM32(0x4002002cu)=0x5a5au;
    RAM32(0x4002002cu)=0xa5a5u;
    RAM32(0x40020030u)=0;
}
void reset_compare_values(void) {
    RAM32(0x40000c3cu)=(RAM32(0x40000c3cu)&0xffff0000u)|1200u;
    RAM32(0x40000c40u)=(RAM32(0x40000c40u)&0xffff0000u)|1200u;
    RAM32(0x40000d3cu)=(RAM32(0x40000d3cu)&0xffff0000u)|1200u;
    RAM32(0x40000d40u)=(RAM32(0x40000d40u)&0xffff0000u)|1200u;
}
void clear_timer_output_field(void) {
    RAM32(0x40000c30u)&=~0x1000u;
    RAM32(0x40000d30u)&=~0x1000u;
}
void set_timer_output_field(void) {
    RAM32(0x40000c30u)|=0x1000u;
    RAM32(0x40000d30u)|=0x1000u;
}
void clock_adc_stop(void) {
    RAM32(0x40021014u)&=~0x80000u;
    RAM32(0x40021014u)&=~0x70000u;
    RAM32(0x400024b4u)&=~0x20u;
}
void clock_adc_set_bit19(void) {
    RAM32(0x40021014u)|=0x80000u;
}
void fault_indicator_write(uint32_t v) {
    if (v)RAM32(0x40020d88u)|=0x4000u;
    else RAM32(0x40020d88u)&=~0x4000u;
}
void fault_indicator_toggle(uint32_t n, uint32_t delay) {
    while (n) {
        if (delay)factory_delay_ms(delay);
        if (RAM32(0x40020d88u)&0x4000u)RAM32(0x40020d88u)&=~0x4000u;
        else RAM32(0x40020d88u)|=0x4000u;
        n=(uint16_t)(n-1);
    }
}
uint32_t read_enable_state(void) {
    if (LIVE->control_mode==2)return RAM8(WORK_RAM(WS_UART_ENABLE_REQUEST));
    if (LIVE->enable_mode==2)return 1;
    uint32_t v=gpio_read_input_bit(0x40, 7);
    return LIVE->enable_mode==1?v:!v;
}
void reset_phase_state(void) {
    RAM8(WORK_RAM(WS_PHASE_PREDICTION_ENABLED))=0;
    RAM16(WORK_RAM(WS_PHASE_INDEX))=0;
    RAM32(WORK_RAM(WS_CURRENT_COMMAND_AXIS_A))=0;
    RAM32(WORK_RAM(WS_CONTROL_OUTPUT))=0;
    factory_reset_compare();
}
void step_input_disable(void) {
    factory_aux_stop(0x40003000u);
    gpio_clear_indexed_field(0x40, 7, 2);
    gpio_clear_indexed_field(0x40, 7, 3);
    gpio_clear_irq_field(0x40, 7);
}
void step_input_enable(void) {
    gpio_clear_irq_field(0x40, 7);
    gpio_set_indexed_field(0x40, 7, 2);
    gpio_set_indexed_field(0x40, 7, 3);
    factory_aux_start(0x40003000u);
}
void pause_control_chain(void) {
    factory_clock_adc_stop();
    factory_step_input_disable();
    timer2_pause_irq();
}
void save_calibration_knots(uint32_t address) {
    factory_flash_erase_write(CALIBRATION_KNOT_PAGE, address, 0xcbu);
}
void boot_banner(void) {
    oled_initialize();
    display_three_lines(TEXT_ADDR(msg_version_mks), TEXT_ADDR(msg_version_servo42c), TEXT_ADDR(msg_version_v1_1_3));
    factory_delay_ms(1000);
}
/* Wrap prediction uses the same signed halfword and machine-word arithmetic. */
uint32_t predict_phase(void) {
    if (!RAM8(WORK_RAM(WS_PHASE_PREDICTION_ENABLED)))return RAM16(WORK_RAM(WS_PHASE_INDEX))&1023u;
    RAM16(WORK_RAM(WS_ENCODER_ANGLE))=encoder_lut_read(factory_read_encoder());
    int32_t diff=(int16_t)(RAM16(WORK_RAM(WS_ENCODER_ANGLE))-RAM16(WORK_RAM(WS_PREVIOUS_ENCODER_ANGLE)));
    RAM16(WORK_RAM(WS_PREVIOUS_ENCODER_ANGLE))=RAM16(WORK_RAM(WS_ENCODER_ANGLE));
    uint32_t phase=RAM16(WORK_RAM(WS_ENCODER_ANGLE))+(uint32_t)diff;
    int32_t correction=(int32_t)RAM32(WORK_RAM(WS_CONTROL_OUTPUT));
    if (correction) {
        int32_t advance=(int32_t)(abs_word((uint32_t)correction)+64u)>>7;
        if (advance>49)advance=49;
        phase+=correction<1?0u-(uint32_t)advance:(uint32_t)advance;
    }
    return ((uint32_t)(uint16_t)phase*MOTOR->motor_geometry+16u)>>5&1023u;
}
void render_menu_rows(void) {
    char rows[32];
    fill_bytes((uint32_t)(uintptr_t)rows, 32, 0);
    uint32_t table=RAM32(WORK_RAM(WS_MENU_TABLE_ADDRESS)), options=RAM32(WORK_RAM(WS_MENU_OPTIONS_ADDRESS));
    unsigned selected=RAM8(WORK_RAM(WS_MENU_SELECTED_OPTION));
    unsigned i;
    if (!options) {
        for (i=0;i<3;i++) {
            uint32_t entry=table+20u*(RAM8(WORK_RAM(WS_MENU_STATE))+i);
            if (!RAM32(entry+12))break;
            rows[i*10]=' ';
            string_copy((uint32_t)(uintptr_t)(rows+i*10+1), entry);
        }
        rows[0]='*';
    }
    else if (selected<3) {
        for (i=0;i<3;i++) {
            if (!factory_strlen(options+i*9))break;
            rows[i*10]=' ';
            string_copy((uint32_t)(uintptr_t)(rows+i*10+1), options+i*9);
        }
        rows[selected*10]='*';
    }
    else {
        for (i=0;i<3;i++) {
            rows[i*10]=' ';
            string_copy((uint32_t)(uintptr_t)(rows+i*10+1), options+(selected+i-2)*9);
        }
        rows[20]='*';
    }
    display_three_lines((uint32_t)(uintptr_t)rows, (uint32_t)(uintptr_t)(rows+10), (uint32_t)(uintptr_t)(rows+20));
}
