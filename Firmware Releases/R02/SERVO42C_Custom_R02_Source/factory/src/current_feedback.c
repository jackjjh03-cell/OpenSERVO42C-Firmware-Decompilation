#include "factory_layout.h"
/* ADC feedback and startup offset validation. Out-of-range current offsets
 * stop startup; the thresholds are in raw ADC counts. */
extern void adc_start(void), adc_stop(void), adc_clear_flags(uint32_t);
extern uint32_t adc_test_flags(uint32_t), adc_read_result(uint32_t);
extern void factory_delay_ms(uint32_t), timer_clear_all(uint32_t);
extern uint32_t timer_clear_flag(uint32_t, uint32_t), timer_config_20(uint32_t, uint32_t);
extern void adc_interrupt_mask(uint32_t, uint32_t);
extern void display_three_lines(uint32_t, uint32_t, uint32_t);
extern void adc_shutdown_chain(void);
extern uint32_t abs_word(uint32_t);
void check_current_offsets(uint32_t ignored_r0, uint32_t caller_r1) {
    (void)ignored_r0;
    int32_t min_a=0, max_a=0, min_b=0, max_b=0;
    for (unsigned i=0;i<18;i++) {
        adc_start();
        factory_delay_ms(5);
        while (!adc_test_flags(0x20)) {
        }
        int32_t a=(int32_t)adc_read_result(2), b=(int32_t)adc_read_result(1);
        if (!i) {
            min_a=max_a=a;
            min_b=max_b=b;
        }
        else {
            if (a<min_a)min_a=a;
            else if (a>max_a)max_a=a;
            if (b<min_b)min_b=b;
            else if (b>max_b)max_b=b;
        }
        RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_2))+=(uint32_t)a;
        RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_1))+=(uint32_t)b;
        adc_clear_flags(0x20);
    }
    adc_stop();
    RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_2))=(uint32_t)((int32_t)(RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_2))-(uint32_t)min_a-(uint32_t)max_a+8u)>>4);
    RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_1))=(uint32_t)((int32_t)(RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_1))-(uint32_t)min_b-(uint32_t)max_b+8u)>>4);
    if ((int32_t)RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_2))<1861||(int32_t)RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_2))>2234||(int32_t)RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_1))<1861||(int32_t)RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_1))>2234) {
        display_three_lines(TEXT_ADDR(msg_current_offset_offset), TEXT_ADDR(msg_current_offset_current), TEXT_ADDR(msg_current_offset_error));
        for (;;) {
        }
    }
 /* The first two timer configuration bytes are fixed; the remaining two are
 * taken from the upper half of caller_r1. The startup caller supplies zero. */
    uint8_t config[4]={1, 1, (uint8_t)(caller_r1>>16), (uint8_t)(caller_r1>>24)};
    timer_config_20(0, (uint32_t)(uintptr_t)config);
    timer_clear_all(0);
    timer_clear_flag(0, 0);
    adc_interrupt_mask(1, 1);
}
void startup_adc_irq(void) {
    if (RAM32(0x400024b0u)&0x20u) {
        if ((int32_t)adc_read_result(0)<745)RAM16(WORK_RAM(WS_STARTUP_FEEDBACK_COUNT))=65535;
        else {
            if (RAM16(WORK_RAM(WS_STARTUP_FEEDBACK_COUNT))==65535)RAM16(WORK_RAM(WS_STARTUP_FEEDBACK_COUNT))=0;
            if (RAM16(WORK_RAM(WS_STARTUP_FEEDBACK_COUNT))>99 && RAM16(WORK_RAM(WS_STARTUP_FEEDBACK_COUNT))<116) {
                RAM32(WORK_RAM(WS_STARTUP_FEEDBACK_SUM_2))+=adc_read_result(2);
                RAM32(WORK_RAM(WS_STARTUP_FEEDBACK_SUM_1))+=adc_read_result(1);
                if (RAM16(WORK_RAM(WS_STARTUP_FEEDBACK_COUNT))>114) {
                    adc_shutdown_chain();
                    RAM32(WORK_RAM(WS_STARTUP_FEEDBACK_SUM_2))=abs_word((uint32_t)((int32_t)RAM32(WORK_RAM(WS_STARTUP_FEEDBACK_SUM_2))>>4)-RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_2)));
                    RAM32(WORK_RAM(WS_STARTUP_FEEDBACK_SUM_1))=abs_word((uint32_t)((int32_t)RAM32(WORK_RAM(WS_STARTUP_FEEDBACK_SUM_1))>>4)-RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_1)));
                    RAM8(WORK_RAM(WS_STARTUP_FEEDBACK_READY))=1;
                }
            }
            RAM16(WORK_RAM(WS_STARTUP_FEEDBACK_COUNT))++;
            if (RAM16(WORK_RAM(WS_STARTUP_FEEDBACK_COUNT))>115)RAM16(WORK_RAM(WS_STARTUP_FEEDBACK_COUNT))=116;
        }
    }
    RAM32(0x400024b4u)&=~0x20u;
}
extern void factory_irq_config(uint32_t, uint32_t, uint32_t);
extern void reset_compare_values(void);
extern void feedback_difference(uint32_t), rotate_q14(uint32_t, uint32_t, uint32_t, uint32_t);
extern uint32_t predict_phase(void);
extern int32_t scaled_difference_a(uint32_t, uint32_t), scaled_difference_b(uint32_t, uint32_t);
extern void inverse_rotate_q14(uint32_t, uint32_t, uint32_t);
extern uint32_t hardware_divide(uint32_t, uint32_t, uint32_t), factory_divide(uint32_t, uint32_t);
extern void motor_pwm_write(uint32_t, uint32_t), current_control_update(void);
/* startup ADC IRQ gate. */
void adc_shutdown_chain(void) {
    RAM32(0x40002404u)&=~0x8000u;
    factory_irq_config(24, 0, 0);
    RAM32(0x40021014u)&=~0x70000u;
    RAM32(0x400024b4u)&=~0x20u;
}
void adc_startup_irq_enable(void) {
    factory_irq_config(24, 0, 1);
    RAM32(0x40002404u)|=0x8000u;
}
/* hardware unsigned division, original error/poll semantics. */
uint32_t hardware_divide(uint32_t numerator, uint32_t denominator, uint32_t out) {
    RAM32(0x40021810u)&=~1u;
    if (!out)return 4;
    RAM32(0x40021800u)=numerator;
    RAM32(0x40021804u)=denominator;
    if (RAM32(0x40021814u)&2u)return 4;
    while (!(RAM32(0x40021814u)&1u)) {
    }
    RAM32(out)=RAM32(0x40021808u);
    RAM32(out+4)=RAM32(0x4002180cu);
    return 0;
}
/* scale the two signed commands to the original compare counts.
 * Retains the original clamp, flag-specific 600 minimum and ordering. */
void motor_pwm_write(uint32_t a, uint32_t b) {
    uint32_t quotient[2];
    uint32_t n=(uint32_t)((int32_t)RAM32(WORK_RAM(WS_CURRENT_SCALE_RAW))>>1)+0x12c000u;
    uint32_t q;
    if (hardware_divide(n, (uint16_t)RAM32(WORK_RAM(WS_CURRENT_SCALE_RAW)), (uint32_t)(uintptr_t)quotient))
    q=factory_divide((uint32_t)((int32_t)RAM32(WORK_RAM(WS_CURRENT_SCALE_RAW))>>1)+0x12c000u, RAM32(WORK_RAM(WS_CURRENT_SCALE_RAW)));
    else q=quotient[0];
    uint32_t x=((q*abs_word(a)+512u)&0x3ffffffu)>>10;
    if (LIVE->torque_raw<x)x=LIVE->torque_raw;
    x=(uint16_t)(1200u-x);
    uint32_t y=((q*abs_word(b)+512u)&0x3ffffffu)>>10;
    if (LIVE->torque_raw<y)y=LIVE->torque_raw;
    y=(uint16_t)(1200u-y);
    if (RAM32(WORK_RAM(WS_STARTUP_FLAG))==1) {
        if (x<600)x=600;
        if (y<600)y=600;
    }
    if ((int32_t)a<0) {
        RAM32(0x40000c3cu)=(RAM32(0x40000c3cu)&0xffff0000u)|1200u;
        RAM32(0x40000c40u)=(RAM32(0x40000c40u)&0xffff0000u)|x;
    }
    else {
        RAM32(0x40000c40u)=(RAM32(0x40000c40u)&0xffff0000u)|1200u;
        RAM32(0x40000c3cu)=(RAM32(0x40000c3cu)&0xffff0000u)|x;
    }
    if ((int32_t)b<0) {
        RAM32(0x40000d40u)=(RAM32(0x40000d40u)&0xffff0000u)|1200u;
        RAM32(0x40000d3cu)=(RAM32(0x40000d3cu)&0xffff0000u)|y;
    }
    else {
        RAM32(0x40000d3cu)=(RAM32(0x40000d3cu)&0xffff0000u)|1200u;
        RAM32(0x40000d40u)=(RAM32(0x40000d40u)&0xffff0000u)|y;
    }
}
/* sampled current feedback, rotation, limited control and PWM. */
void current_control_update(void) {
    RAM32(WORK_RAM(WS_CURRENT_SCALE_RAW))=RAM32(WORK_RAM(WS_ADC_DMA_SAMPLE_RAW));
    if ((int32_t)RAM32(WORK_RAM(WS_CURRENT_SCALE_RAW))<745) {
        reset_compare_values();
        return;
    }
    RAM32(WORK_RAM(WS_CURRENT_POSITIVE_LIMIT))=(uint32_t)((int32_t)(RAM32(WORK_RAM(WS_CURRENT_SCALE_RAW))*972u+512u)>>10);
    RAM32(WORK_RAM(WS_CURRENT_NEGATIVE_LIMIT))=0u-RAM32(WORK_RAM(WS_CURRENT_POSITIVE_LIMIT));
    uint32_t feedback[2], axes[2];
    feedback_difference((uint32_t)(uintptr_t)feedback);
    uint32_t phase=predict_phase();
    rotate_q14((uint32_t)(uintptr_t)axes, feedback[0], feedback[1], phase);
    uint32_t a=(uint32_t)scaled_difference_b(RAM32(WORK_RAM(WS_CURRENT_COMMAND_AXIS_A)), axes[0]);
    uint32_t b=(uint32_t)scaled_difference_a(RAM32(WORK_RAM(WS_CONTROL_OUTPUT)), axes[1]);
    inverse_rotate_q14((uint32_t)(uintptr_t)axes, a, b);
    motor_pwm_write(axes[0], axes[1]);
}
/* ADC update dispatcher and original status clears. */
void current_adc_irq(void) {
    if (((RAM32(0x40021014u)>>16)&7u)==5)current_control_update();
    RAM32(0x40021014u)&=~0x70000u;
    RAM32(0x400024b4u)&=~0x20u;
}
