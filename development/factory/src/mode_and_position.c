#include "factory_layout.h"
extern void fault_indicator_write(uint32_t), reset_phase_state(void), display_three_lines(uint32_t, uint32_t, uint32_t), factory_delay_ms(uint32_t), menu_select_table(uint32_t), menu_request_redraw(void);
extern uint32_t encoder_startup_status(void), load_calibration_knots(void), encoder_trimmed_mean(uint32_t);
extern uint16_t encoder_lut_read(uint32_t);
extern uint64_t factory_multiply64(uint64_t, uint64_t), factory_scaled_position(uint64_t, uint32_t);
extern int32_t factory_signed_divide(int32_t, int32_t);
/* Preserve all encoder fault messages and calibration failure branches.
 * Original strings at 0x57E8..0x582F deliberately not replaced. */
uint32_t factory_apply_mode(uint32_t mode) {
    fault_indicator_write(0);
    reset_phase_state();
    if (mode!=2)RAM8(WORK_RAM(WS_UART_ENABLE_REQUEST))=1;
    if (!mode) {
        RAM32(WORK_RAM(WS_POSITION_ERROR))=0;
        RAM8(WORK_RAM(WS_PHASE_PREDICTION_ENABLED))=0;
    }
    else {
        RAM8(WORK_RAM(WS_PHASE_PREDICTION_ENABLED))=1;
        uint32_t result=0, table=0;
        if (encoder_startup_status()) {
            uint32_t a=TEXT_ADDR(msg_mode_encoder), b=TEXT_ADDR(msg_mode_error);
            uint8_t status=RAM8(WORK_RAM(WS_ENCODER_STATUS));
            if (status>=1&&status<=3) {
                a=TEXT_ADDR(msg_mode_magnet);
                b=status==1?TEXT_ADDR(msg_mode_strong):status==2?TEXT_ADDR(msg_mode_weak):TEXT_ADDR(msg_mode_loss);
            }
            display_three_lines(a, b, TEXT_ADDR(msg_mode_enter));
            table=WORK_RAM(0x0560u);
            result=3;
        }else if (!load_calibration_knots()) {
            display_three_lines(TEXT_ADDR(msg_mode_blank), TEXT_ADDR(msg_mode_not_cal), TEXT_ADDR(msg_mode_blank));
            table=WORK_RAM(0x04fcu);
            result=2;
        }
        if (result) {
            factory_delay_ms(1200);
            RAM32(WORK_RAM(WS_MENU_OPTIONS_ADDRESS))=0;
            menu_select_table(table);
            menu_request_redraw();
            return result;
        }
    }
    menu_select_table(WORK_RAM(0x0358u));
    return 0;
}
/* Reset pulse/position tracking while retaining calibrated position. */
void reset_position_state(void) {
    reset_phase_state();
    volatile uint32_t *p=PTR(uint32_t, WORK_RAM(WS_MOTION_POSITION)), *s=PTR(uint32_t, WORK_RAM(WS_MOTION_FEEDBACK));
    volatile uint16_t *h=PTR(uint16_t, WORK_RAM(WS_MOTION_FEEDBACK));
    p[0]=0;
    p[1]=0;
    p[4]=0;
    p[5]=0;
    s[12]=0;
    s[11]=0;
    h[1]=0;
    RAM32(0x40003000u)&=0xffff0000u;
    RAM32(WORK_RAM(WS_UART_TARGET_LOW))=0;
    RAM32(WORK_RAM(WS_UART_TARGET_HIGH))=0;
    RAM32(WORK_RAM(0x061cu))=0;
    if (LIVE->control_mode&&RAM16(WORK_RAM(WS_CALIBRATION_LUT_VALID))==1) {
        h[2]=(uint16_t)encoder_lut_read(encoder_trimmed_mean(3));
        p[4]=h[2];
        p[5]=0;
        h[3]=h[2];
        uint32_t n=(uint32_t)h[2]*MOTOR->motor_geometry+(uint32_t)((int32_t)s[3]>>1);
        int32_t q=factory_signed_divide((int32_t)n, (int32_t)s[3]);
        p[0]=(uint32_t)q;
        p[1]=(uint32_t)(q>>31);
        RAM8(WORK_RAM(WS_PHASE_PREDICTION_ENABLED))=1;
    }
    uint64_t product=factory_multiply64((uint64_t)p[0]|((uint64_t)p[1]<<32), (uint64_t)(int64_t)(int32_t)s[3]);
    uint64_t value=factory_scaled_position(product, MOTOR->motor_geometry);
    p[8]=(uint32_t)value;
    p[9]=(uint32_t)(value>>32);
    s[7]=p[0];
    s[8]=p[4];
}
