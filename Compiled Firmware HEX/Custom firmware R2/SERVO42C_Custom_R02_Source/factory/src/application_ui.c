#include "factory_layout.h"
#include "sysinfo.h"
/* foreground UI, display sleep and stall acknowledgement.
 * Original stall acknowledgement sequence and button latches retained. */
#define BUTTON WORK_RAM(0x05dcu)
#define MENU WORK_RAM(WS_MENU_STATE)
#define STATE WORK_RAM(WS_MOTION_FEEDBACK)
volatile uint8_t magnet_lock;
extern void rpm_reset(void), rpm_toggle(void), uart_disable(void);
extern void fault_indicator_write(uint32_t), factory_oled_command(uint32_t), display_three_lines(uint32_t, uint32_t, uint32_t), update_multiturn_angle(void), reset_position_state(void), step_input_enable(void), factory_refresh_mode(void), menu_select_table(uint32_t), factory_delay_ms(uint32_t), boot_banner(void), render_telemetry(void), menu_poll_buttons(void);
extern uint32_t gpio_read_input_bit(uint32_t, uint32_t), factory_timer_reload(uint32_t, uint32_t), factory_timer_start(uint32_t), factory_timer_stop(uint32_t);
void factory_foreground_ui(void) {
    if (!magnet_lock && LIVE->protection==1 && RAM8(STATE+1)) {
        rpm_reset();
        sysinfo_cancel();
        fault_indicator_write(1);
        RAM32(MENU+4)=WORK_RAM(0x059cu);
        RAM16(BUTTON+4)=0;
        if (RAM8(BUTTON)) {
            factory_oled_command(0xaf);
            RAM8(BUTTON)=0;
        }
        display_three_lines(TEXT_ADDR(msg_protection_wrong), TEXT_ADDR(msg_protection_protect), TEXT_ADDR(msg_protection_enter));
        while (gpio_read_input_bit(0, 10)==1) {
            update_multiturn_angle();
            if (RAM8(WORK_RAM(0x05d8u))==1) {
                RAM8(WORK_RAM(0x05d8u))=0;
                break;
            }
        }
        RAM8(STATE+1)=0;
        RAM8(WORK_RAM(WS_UART_ENABLE_REQUEST))=0;
        reset_position_state();
        step_input_enable();
        factory_refresh_mode();
        RAM32(STATE+0x24)=0;
        menu_select_table(WORK_RAM(0x0358u));
        RAM8(MENU+2)=0;
        RAM8(BUTTON+1)=0;
        factory_delay_ms(32);
    }
    if (!RAM8(BUTTON)||RAM8(STATE+1)) {
        if (RAM16(BUTTON+2)>=10) {
            sysinfo_cancel();
            boot_banner();
            RAM16(BUTTON+2)=0;
            RAM8(BUTTON)=0;
            RAM16(BUTTON+4)=0;
        }
        if ((0u-RAM8(MENU+2))&RAM32(MENU+4)){rpm_reset();menu_poll_buttons();}
        else {
            if (!magnet_lock) {
                if (!gpio_read_input_bit(0,9)) {
                    if (!(RAM8(BUTTON+1)&1u))rpm_toggle();
                    RAM8(BUTTON+1)|=1;
                }else RAM8(BUTTON+1)&=(uint8_t)~1u;
            }
            render_telemetry();
        }
        if (!gpio_read_input_bit(64, 4)&&!(RAM8(BUTTON+1)&4u)) {
            RAM8(BUTTON+1)|=4;
            RAM16(BUTTON+4)=0;
            RAM8(MENU+2)^=1;
            if (!RAM8(MENU+2)) {
                sysinfo_cancel();
                RAM32(MENU+8)=0;
                RAM8(MENU+1)=0;
                RAM8(MENU)=0;
            }
        }
        if (gpio_read_input_bit(64, 4)==1)RAM8(BUTTON+1)&=(uint8_t)~4u;
        if (!magnet_lock && LIVE->control_mode!=2) {
            if (LIVE->autosdd==1 && !RAM16(BUTTON+4) && RAM32(MENU+4)==WORK_RAM(0x0358u)) {
                RAM16(BUTTON+4)=1;
                factory_timer_reload(2, 0xfe89);
                factory_timer_start(2);
            }
            else if (RAM16(BUTTON+4)>=4096) {
                factory_timer_stop(2);
                sysinfo_cancel();
                rpm_reset();
                RAM8(BUTTON)=1;
                factory_oled_command(0xae);
                RAM16(BUTTON+4)=0;
                RAM8(BUTTON+1)=7;
            }
        }
        return;
    }
    if (!gpio_read_input_bit(64, 4)||!gpio_read_input_bit(0, 9)||!gpio_read_input_bit(0, 10)) {
        RAM8(BUTTON+1)=7;
        RAM8(BUTTON)=0;
        RAM16(BUTTON+4)=0;
        factory_oled_command(0xaf);
    }
}
extern uint32_t startup_prepare(void), load_calibration_knots(void), origin_return(uint32_t);
extern void menu_request_redraw(void), reset_compare_values(void), factory_apply_uart(void), initialize_motion_tick(void), initialize_step_inputs(void);
/* original startup diagnostic state machine. Retains the
 * power/phase/encoder/calibration branches and blocking acknowledgement. */
static __attribute__((noinline,noreturn)) void magnet_settings_only(void) {
    magnet_lock=1;
    /* Disable motor, step-input, ADC and UART interrupt paths before UI access. */
    RAM32(0xe000e180u)=0x01000072u;
    RAM32(0xe000e280u)=0x01000072u;
    RAM8(WORK_RAM(WS_UART_ENABLE_REQUEST))=0;
    RAM8(WORK_RAM(WS_UART_MOTION_STATE))=0;
    factory_refresh_mode();
    uart_disable();
    fault_indicator_write(1);
    unsigned status=RAM8(WORK_RAM(WS_ENCODER_STATUS));
    if (status>=1&&status<=3)display_three_lines(TEXT_ADDR(msg_startup_magnet), status==1?TEXT_ADDR(msg_startup_strong):status==2?TEXT_ADDR(msg_startup_weak):TEXT_ADDR(msg_startup_loss), TEXT_ADDR("Menu"));
    else display_three_lines(TEXT_ADDR(msg_startup_encoder), TEXT_ADDR(msg_startup_error_0f90), TEXT_ADDR("Menu"));
    /* Menu may dismiss the warning early; no automatic drive recovery. */
    for(unsigned i=0;i<1000;i++){
        if(!gpio_read_input_bit(64,4)||!gpio_read_input_bit(0,10))break;
        factory_delay_ms(10);
    }
    menu_select_table(WORK_RAM(0x0358u));
    RAM32(MENU+8)=0;
    RAM8(MENU+2)=1;
    RAM8(BUTTON+1)=7;
    for(;;)factory_foreground_ui();
}
void factory_startup_diagnostics(void) {
    boot_banner();
    RAM8(STATE)=2;
    while (RAM8(STATE)) {
        RAM8(STATE)=(uint8_t)startup_prepare();
        if (RAM8(STATE)==4) {
            display_three_lines(TEXT_ADDR(msg_startup_phase), TEXT_ADDR(msg_startup_line), TEXT_ADDR(msg_startup_error));
            while (RAM8(STATE)==4)RAM8(STATE)=(uint8_t)startup_prepare();
        }
        if (RAM8(STATE)==3) {
            magnet_settings_only();
        }
        if (RAM8(STATE)==1) {
            display_three_lines(TEXT_ADDR(msg_startup_motor), TEXT_ADDR(msg_startup_type), TEXT_ADDR(msg_startup_error));
            while (RAM8(STATE)==1)RAM8(STATE)=(uint8_t)startup_prepare();
        }
        if (RAM8(STATE)==2) {
            display_three_lines(TEXT_ADDR(msg_startup_blank), TEXT_ADDR(msg_startup_not_cal), TEXT_ADDR(msg_startup_blank));
            factory_delay_ms(1200);
            RAM32(MENU+8)=0;
            menu_select_table(WORK_RAM(0x04fcu));
            menu_request_redraw();
            reset_compare_values();
            while (LIVE->control_mode&&!load_calibration_knots())factory_foreground_ui();
        }
    }
    factory_apply_uart();
    initialize_motion_tick();
    origin_return(1);
    initialize_step_inputs();
    menu_select_table(WORK_RAM(0x0358u));
    RAM8(MENU+2)=0;
}
