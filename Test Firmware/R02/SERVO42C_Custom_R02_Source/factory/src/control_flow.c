#include "factory_layout.h"
extern uint32_t read_enable_state(void), encoder_trimmed_mean(uint32_t);
extern uint16_t encoder_lut_read(uint32_t);
extern void reset_position_state(void), set_timer_output_field(void), clock_adc_set_bit19(void), factory_aux_start(uint32_t), timer2_resume_irq(void), factory_aux_stop(uint32_t), timer2_pause_irq(void), clock_adc_stop(void), clear_timer_output_field(void), reset_compare_values(void), fault_indicator_write(uint32_t), step_input_enable(void), factory_uart_tx_end(void), factory_pause_control(void), copy_runtime_settings(void), factory_commit(void), display_three_lines(uint32_t, uint32_t, uint32_t), factory_delay_ms(uint32_t);
extern uint32_t timer_clear_flag(uint32_t, uint32_t), timer_set_400(uint32_t);
extern uint32_t gpio_read_irq_bit(uint32_t, uint32_t), gpio_clear_irq_field(uint32_t, uint32_t);
extern uint32_t origin_return(uint32_t);
void factory_refresh_mode(void) {
    if (read_enable_state()) {
        reset_position_state();
        set_timer_output_field();
        clock_adc_set_bit19();
        factory_aux_start(0x40003000u);
        timer2_resume_irq();
        RAM8(WORK_RAM(WS_DRIVE_ENABLE_STATE))=1;
    }
    else {
        factory_aux_stop(0x40003000u);
        timer2_pause_irq();
        clock_adc_stop();
        clear_timer_output_field();
        reset_compare_values();
        fault_indicator_write(0);
        RAM8(WORK_RAM(WS_DRIVE_ENABLE_STATE))=0;
    }
}
void enable_input_irq(void) {
    if (gpio_read_irq_bit(0x40, 7)==1) {
        if (RAM8(WORK_RAM(WS_RUNTIME_ENABLE_MODE))!=2&&RAM8(WORK_RAM(WS_RUNTIME_CONTROL_MODE))!=2)factory_refresh_mode();
        gpio_clear_irq_field(0x40, 7);
    }
}
void restart_auxiliary_timer(void) {
    timer_clear_flag(2, 0);
    timer_set_400(2);
}
extern volatile uint8_t magnet_lock;
void factory_resume_control(void) {
    if(magnet_lock)return;
    factory_refresh_mode();
    if (RAM8(WORK_RAM(WS_RUNTIME_CONTROL_MODE))==2) {
        if (RAM8(WORK_RAM(WS_UART_MOTION_STATE))==1&&RAM16(WORK_RAM(WS_SAVED_UART_MOTION))!=65535) {
            RAM32(WORK_RAM(0x061cu))=0;
            RAM32(WORK_RAM(WS_UART_TARGET_LOW))=0;
            RAM32(WORK_RAM(WS_UART_TARGET_HIGH))=0;
            RAM8(WORK_RAM(WS_UART_MOTION_DIRECTION))=(uint8_t)(RAM16(WORK_RAM(WS_SAVED_UART_MOTION))>>15);
            RAM16(WORK_RAM(WS_UART_MOTION_SPEED))=RAM16(WORK_RAM(WS_SAVED_UART_MOTION))&32767u;
        }
    }else step_input_enable();
    restart_auxiliary_timer();
    factory_uart_tx_end();
}
void origin_failure_message(void) {
    display_three_lines(TEXT_ADDR(msg_origin_failure_back_to), TEXT_ADDR(msg_origin_failure_origin), TEXT_ADDR(msg_origin_failure_fail));
    fault_indicator_write(1);
    factory_delay_ms(1000);
}
uint32_t factory_menu_set_origin(uint32_t unused) {
    if(magnet_lock)return 0;
    (void)unused;
    if (!RAM8(WORK_RAM(WS_RUNTIME_CONTROL_MODE))) {
        display_three_lines(TEXT_ADDR(msg_set_origin_it_is), TEXT_ADDR(msg_set_origin_cr_open), TEXT_ADDR(msg_set_origin_mode));
        factory_delay_ms(1200);
        return 0;
    }
    if (!RAM8(WORK_RAM(WS_ORIGIN_SETTINGS))) {
        display_three_lines(TEXT_ADDR(msg_set_origin_0_mode), TEXT_ADDR(msg_set_origin_is), TEXT_ADDR(msg_set_origin_disable));
        factory_delay_ms(1200);
        return 0;
    }
    factory_pause_control();
    copy_runtime_settings();
    RAM16(WORK_RAM(0x0718u))=(uint16_t)encoder_lut_read(encoder_trimmed_mean(3));
    factory_commit();
    display_three_lines(TEXT_ADDR(msg_set_origin_origin), TEXT_ADDR(msg_set_origin_set), TEXT_ADDR(msg_set_origin_done));
    factory_delay_ms(1200);
    return 1;
}
uint32_t factory_menu_goto_origin(uint32_t unused) {
    if(magnet_lock)return 0;
    (void)unused;
    uint32_t a, b, c;
    if (!RAM8(WORK_RAM(WS_RUNTIME_CONTROL_MODE))) {
        a=TEXT_ADDR(msg_origin_mode_it_is);
        b=TEXT_ADDR(msg_origin_mode_cr_open);
        c=TEXT_ADDR(msg_origin_mode_mode);
    }
    else if (!RAM8(WORK_RAM(WS_ORIGIN_SETTINGS))) {
        a=TEXT_ADDR(msg_origin_mode_0_mode);
        b=TEXT_ADDR(msg_origin_mode_is);
        c=TEXT_ADDR(msg_origin_mode_disable);
    }
    else if (RAM16(WORK_RAM(0x05f8u))==65535) {
        a=TEXT_ADDR(msg_origin_mode_origin);
        b=TEXT_ADDR(msg_origin_mode_not);
        c=TEXT_ADDR(msg_origin_mode_set);
    }
    else {
        if (read_enable_state())return origin_return(0);
        reset_position_state();
        uint8_t save=RAM8(WORK_RAM(WS_RUNTIME_ENABLE_MODE));
        RAM8(WORK_RAM(WS_RUNTIME_ENABLE_MODE))=2;
        RAM8(WORK_RAM(WS_UART_ENABLE_REQUEST))=1;
        factory_refresh_mode();
        factory_delay_ms(10);
        uint32_t v=origin_return(0);
        RAM8(WORK_RAM(WS_UART_ENABLE_REQUEST))=0;
        RAM8(WORK_RAM(WS_RUNTIME_ENABLE_MODE))=save;
        factory_refresh_mode();
        return v;
    }
    display_three_lines(a, b, c);
    factory_delay_ms(1200);
    return 0;
}
