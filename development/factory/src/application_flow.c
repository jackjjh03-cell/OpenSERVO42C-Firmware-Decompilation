#include "factory_layout.h"
/* Initialize the board before checking device identity. Enter the foreground
 * loop only after startup diagnostics complete. */
extern void factory_system_init(void), factory_board_init(void), factory_config_pins(void), factory_startup_diagnostics(void), factory_foreground_ui(void), update_multiturn_angle(void), factory_refresh_mode(void), deferred_origin_return(void), deferred_origin_set(void);
extern uint32_t factory_identity_check(void);
extern void fault_indicator_toggle(uint32_t, uint32_t);
void application_main(void) {
    factory_system_init();
    RAM32(WORK_RAM(WS_STARTUP_FLAG))=1;
    factory_board_init();
    uint32_t ok=factory_identity_check();
    factory_config_pins();
    if (!ok)for (;;)fault_indicator_toggle(255, 1000);
    factory_startup_diagnostics();
    RAM32(WORK_RAM(WS_STARTUP_FLAG))=0;
    for (;;) {
        factory_foreground_ui();
        update_multiturn_angle();
        if (RAM8(WORK_RAM(WS_MODE_REFRESH_PENDING))) {
            RAM8(WORK_RAM(WS_MODE_REFRESH_PENDING))=0;
            factory_refresh_mode();
        }
        if (RAM8(WORK_RAM(WS_ORIGIN_RETURN_PENDING)))deferred_origin_return();
        if (RAM8(WORK_RAM(WS_ORIGIN_SET_PENDING)))deferred_origin_set();
    }
}
extern uint32_t sys_gate_bit(uint32_t, uint32_t);
void enable_board_clocks(void) {
    sys_gate_bit(30, 1);
    sys_gate_bit(28, 1);
    sys_gate_bit(6, 1);
    sys_gate_bit(4, 1);
    sys_gate_bit(0, 1);
    sys_gate_bit(8, 1);
    sys_gate_bit(10, 1);
    sys_gate_bit(11, 1);
    sys_gate_bit(16, 1);
    sys_gate_bit(29, 1);
    sys_gate_bit(27, 1);
}
extern void initialize_indicator(void), initialize_buttons(void), initialize_display_bus(void), initialize_encoder_bus(void), initialize_motor_timers(void), initialize_adc(void);
extern uint32_t flash_timing_setup(uint32_t, uint32_t);
void factory_board_init(void) {
    enable_board_clocks();
    initialize_indicator();
    initialize_buttons();
    initialize_display_bus();
    initialize_encoder_bus();
    initialize_motor_timers();
    initialize_adc();
    (void)flash_timing_setup(12, 0);
}
extern uint32_t factory_menu_goto_origin(uint32_t), factory_menu_set_origin(uint32_t);
extern void uart_start_reply(void);
void deferred_origin_return(void) {
    RAM8(WORK_RAM(WS_ORIGIN_RETURN_PENDING))=0;
    RAM8(WORK_RAM(WS_UART_REPLY_STATUS))=factory_menu_goto_origin(0)==1;
    RAM8(WORK_RAM(WS_UART_TX_REMAINING))=2;
    RAM8(WORK_RAM(WS_UART_TX_REVERSE))=(uint8_t)(RAM16(WORK_RAM(WS_RUNTIME_UART_ADDRESS))+RAM8(WORK_RAM(WS_UART_REPLY_STATUS)));
    uart_start_reply();
}
void deferred_origin_set(void) {
    RAM8(WORK_RAM(WS_ORIGIN_SET_PENDING))=0;
    RAM8(WORK_RAM(WS_UART_REPLY_STATUS))=factory_menu_set_origin(0)==1;
    RAM8(WORK_RAM(WS_UART_TX_REMAINING))=2;
    RAM8(WORK_RAM(WS_UART_TX_REVERSE))=(uint8_t)(RAM16(WORK_RAM(WS_RUNTIME_UART_ADDRESS))+RAM8(WORK_RAM(WS_UART_REPLY_STATUS)));
    uart_start_reply();
}
extern uint32_t factory_timer_stop(uint32_t);
/* 0x3A18 stop a commanded motion, preserve state snapshots. */
void factory_stop(void) {
    RAM32(0x4000580cu)&=~0x400u;
    RAM32(WORK_RAM(WS_UART_TARGET_LOW))=0;
    RAM32(WORK_RAM(WS_UART_TARGET_HIGH))=0;
    RAM32(WORK_RAM(0x061cu))=0;
    RAM8(WORK_RAM(WS_UART_MOTION_STATE))=0;
    RAM32(0x4000580cu)|=0x400u;
    RAM16(WORK_RAM(WS_UART_PULSE_COUNTER))=RAM16(WORK_RAM(WS_PREVIOUS_PULSE_COUNT));
    RAM32(WORK_RAM(WS_UART_TARGET_LOW))=RAM32(WORK_RAM(WS_MOTION_POSITION));
    RAM32(WORK_RAM(WS_UART_TARGET_HIGH))=RAM32(WORK_RAM(WS_PULSE_COUNT_HIGH));
    factory_timer_stop(2);
}
