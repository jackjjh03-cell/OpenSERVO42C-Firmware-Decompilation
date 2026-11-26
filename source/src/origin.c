#include "factory_layout.h"
/* return to origin. Volatile delay loop retains iteration counts;
 * compiler timing and concurrent interrupt behavior remain unverified. */
extern int32_t factory_signed_divide(int32_t, int32_t);
extern void set_timer_output_field(void), clock_adc_set_bit19(void), timer2_resume_irq(void), timer2_pause_irq(void), clock_adc_stop(void), clear_timer_output_field(void), reset_compare_values(void), factory_delay_100us(uint32_t), display_three_lines(uint32_t, uint32_t, uint32_t), origin_failure_message(void), fault_indicator_write(uint32_t);
static inline int32_t origin_error(void) {
    return (int16_t)(ORIGIN->position-RAM32(WORK_RAM(WS_FEEDBACK_POSITION_LOW)));
}
static inline unsigned magnitude(int32_t a) {
    return a<0?(unsigned)-a:(unsigned)a;
}
uint32_t origin_return(uint32_t startup) {
    if (RAM8(WORK_RAM(WS_UART_MOTION_STATE))==1)return 0;
    if (!LIVE->control_mode || !ORIGIN->mode || ORIGIN->position==65535)return 1;
    int32_t err=origin_error(), distance=0;
    uint32_t forward=0;
    if (err>100) {
        if (ORIGIN->mode==1) {
            if (!ORIGIN->direction) {
                forward=1;
                distance=err;
            }else distance=65536-err;
        }
        else if (ORIGIN->mode==2) {
            forward=!ORIGIN->direction;
            distance=err;
        }
    }else if (err < -100) {
        if (ORIGIN->mode==1) {
            if (!ORIGIN->direction) {
                forward=1;
                distance=err+65534;
            }else distance=-err;
        }
        else if (ORIGIN->mode==2) {
            forward=ORIGIN->direction!=0;
            distance=-err;
        }
    }
    int32_t scale=(int32_t)RAM32(WORK_RAM(WS_PULSE_SCALE));
    int32_t steps=factory_signed_divide((int32_t)((uint32_t)distance*MOTOR->motor_geometry+(uint32_t)(scale>>1)), scale);
    if (steps<=0)return 1;
    if (startup) {
        set_timer_output_field();
        clock_adc_set_bit19();
        timer2_resume_irq();
    }
    display_three_lines(TEXT_ADDR(msg_origin_progress_coming), TEXT_ADDR(msg_origin_progress_back_to), TEXT_ADDR(msg_origin_progress_origin));
    for (int32_t i=steps;i>0;i--) {
        uint32_t lo=RAM32(WORK_RAM(WS_MOTION_POSITION)), hi=RAM32(WORK_RAM(WS_PULSE_COUNT_HIGH));
        if (forward) {
            lo++;
            hi+=(lo==0);
        }else {
            hi-=(lo==0);
            lo--;
        }
        RAM32(WORK_RAM(WS_MOTION_POSITION))=lo;
        RAM32(WORK_RAM(WS_PULSE_COUNT_HIGH))=hi;
        unsigned speed=ORIGIN->speed;
        volatile uint32_t wait=speed==0?50:speed==1?100:speed==2?300:speed==3?500:800;
        while (wait)wait--;
    }
    RAM8(WORK_RAM(0x05dau))=1;
    uint32_t iterations=0, stable=0;
    err=origin_error();
    while (magnitude(err)>18 && iterations<100000 && stable<6000) {
        int32_t old=err;
        factory_delay_100us(1);
        iterations++;
        err=origin_error();
        stable=magnitude(err-old)<9?stable+1:0;
    }
    if (startup) {
        timer2_pause_irq();
        clock_adc_stop();
        clear_timer_output_field();
        reset_compare_values();
    }
    if (magnitude(err)>100) {
        RAM8(WORK_RAM(0x05dau))=0;
        origin_failure_message();
        return 0;
    }
    fault_indicator_write(0);
    return 1;
}
