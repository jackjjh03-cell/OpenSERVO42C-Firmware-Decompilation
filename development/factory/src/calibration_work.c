#include "factory_layout.h"
extern uint32_t encoder_inverse_lookup(int32_t);
extern void factory_flash_erase_write(uint32_t, uint32_t, uint32_t), factory_flash_write(uint32_t, uint32_t, uint32_t);
extern void fill_bytes(uint32_t, uint32_t, uint8_t);
/* build 64 pages of 256 halfwords. The last page ends at 0xFFFF.
 * Use the original per-board calibration; do not share a table across units. */
void factory_generate_lut(void) {
    uint16_t page[256];
    fill_bytes((uint32_t)(uintptr_t)page, 512, 0);
    for (unsigned i=0;i<16384;i++) {
        page[i&255u]=(uint16_t)encoder_inverse_lookup((int32_t)i);
        if ((i&255u)==255u)factory_flash_erase_write(CALIBRATION_LUT_BASE+2u*(i&~255u), (uint32_t)(uintptr_t)page, 256);
    }
    RAM16(WORK_RAM(WS_CALIBRATION_LUT_VALID))=1;
    factory_flash_write(CALIBRATION_LUT_VALID, WORK_RAM(WS_CALIBRATION_LUT_VALID), 1);
    RAM8(WORK_RAM(WS_ENCODER_LUT_READY))=1;
}
extern void factory_pause_control(void), display_three_lines(uint32_t, uint32_t, uint32_t), reset_phase_state(void), set_timer_output_field(void), set_phase_and_value(uint32_t, uint32_t), clock_adc_set_bit19(void), factory_delay_ms(uint32_t);
extern uint32_t encoder_trimmed_mean(uint32_t);
extern void calibration_step_forward(void), calibration_step_reverse(void), clock_adc_stop(void), calibration_record_bounds(void), menu_select_table(uint32_t), fault_indicator_write(uint32_t), reset_compare_values(void), factory_resume_control(void);
/* Take two sweeps over 200 knots, align samples across encoder wrap, average
 * and commit. Physical calibration requires bench validation. */
uint32_t calibration_run(uint32_t unused) {
    (void)unused;
    factory_pause_control();
    display_three_lines(TEXT_ADDR(msg_calibration_progress_blank), TEXT_ADDR(msg_calibration_progress_cal), TEXT_ADDR(msg_calibration_progress_blank));
    reset_phase_state();
    set_timer_output_field();
    set_phase_and_value(0, 3000);
    clock_adc_set_bit19();
    factory_delay_ms(1200);
    RAM32(WORK_RAM(WS_CALIBRATION_PHASE_ACCUMULATOR))=0;
    for (unsigned i=0;i<200;i++) {
        RAM16(WORK_RAM(WS_CALIBRATION_KNOTS)+2*i)=(uint16_t)encoder_trimmed_mean(102);
        calibration_step_forward();
    }
    for (unsigned i=200;i>0;i--) {
        calibration_step_reverse();
        int32_t b=(int32_t)encoder_trimmed_mean(102), a=RAM16(WORK_RAM(WS_CALIBRATION_KNOTS)+2*(i-1));
        if (a-b>8192)b+=16384;
        else if (b-a>8192)a+=16384;
        uint32_t m=((uint32_t)a+(uint32_t)b)>>1;
        if (m>16383)m-=16384;
        RAM16(WORK_RAM(WS_CALIBRATION_KNOTS)+2*(i-1))=(uint16_t)m;
    }
    RAM32(WORK_RAM(WS_CALIBRATION_PHASE_ACCUMULATOR))=0;
    set_phase_and_value(0, 3000);
    factory_delay_ms(1200);
    clock_adc_stop();
    calibration_record_bounds();
    if (RAM32(0x4000580cu)&1u) {
        RAM8(WORK_RAM(WS_MOTION_FEEDBACK))=0;
        menu_select_table(WORK_RAM(0x0358u));
        RAM8(WORK_RAM(WS_MENU_REDRAW_FLAG))=0;
    }
    fault_indicator_write(0);
    if (RAM32(0x4000580cu)&1u)factory_resume_control();
    else reset_compare_values();
    return 1;
}
/* step the calibration phase in 128-count increments. */
static inline __attribute__((always_inline)) void sweep_step(uint32_t delta) {
    RAM32(WORK_RAM(WS_CALIBRATION_PHASE_ACCUMULATOR))+=delta;
    set_phase_and_value(RAM16(WORK_RAM(WS_CALIBRATION_PHASE_ACCUMULATOR)), 3000);
    factory_delay_ms(50);
}
void calibration_step_forward(void) {
    sweep_step(128);
    sweep_step(128);
    if (RAM16(WORK_RAM(0x05e8u))==50) {
        sweep_step(128);
        sweep_step(128);
    }
}
void calibration_step_reverse(void) {
    sweep_step(0u-128);
    sweep_step(0u-128);
    if (RAM16(WORK_RAM(0x05e8u))==50) {
        sweep_step(0u-128);
        sweep_step(0u-128);
    }
}
int32_t measure_motor_geometry(void) {
    set_phase_and_value(0, 3000);
    factory_delay_ms(600);
    uint32_t a=encoder_trimmed_mean(102);
    set_phase_and_value(128, 3000);
    factory_delay_ms(120);
    set_phase_and_value(256, 3000);
    factory_delay_ms(600);
    uint32_t b=encoder_trimmed_mean(102);
    set_phase_and_value(128, 3000);
    factory_delay_ms(120);
    set_phase_and_value(0, 3000);
    factory_delay_ms(600);
    reset_compare_values();
    return (int16_t)(b-a);
}
extern void factory_stop(void);
uint32_t factory_calibrate(uint32_t unused) {
    (void)unused;
    if (RAM8(WORK_RAM(WS_RUNTIME_CONTROL_MODE))==0) {
        display_three_lines(TEXT_ADDR(msg_calibration_it_is), TEXT_ADDR(msg_calibration_cr_open), TEXT_ADDR(msg_calibration_mode));
        factory_delay_ms(1200);
        return 0;
    }
    uint16_t m=RAM16(WORK_RAM(WS_RUNTIME_SETTINGS)), c=RAM16(WORK_RAM(WS_EXTRA_SETTINGS));
    RAM16(WORK_RAM(WS_RUNTIME_SETTINGS))=16;
    RAM16(WORK_RAM(WS_EXTRA_SETTINGS))=286;
    if (RAM8(WORK_RAM(WS_RUNTIME_CONTROL_MODE))==2) {
        factory_stop();
        factory_pause_control();
        display_three_lines(TEXT_ADDR(msg_calibration_blank), TEXT_ADDR(msg_calibration_cal), TEXT_ADDR(msg_calibration_blank));
    }
    uint32_t v=calibration_run(0);
    RAM16(WORK_RAM(WS_RUNTIME_SETTINGS))=m;
    RAM16(WORK_RAM(WS_EXTRA_SETTINGS))=c;
    return v;
}
