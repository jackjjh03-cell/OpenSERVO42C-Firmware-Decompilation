#include "factory_layout.h"
/* recovered motion timer ISR. Retains factory memory ABI and output
 * protection. Unsigned machine-word arithmetic deliberately wraps. The
 * fixed-point units and timing are still subject to physical validation. */
typedef struct {
    uint64_t pulse_count, target, encoder, previous_encoder, interpolated_target;
}
MotionPosition;
typedef struct {
    uint8_t startup_state, stalled;
    uint16_t previous_pulse, encoder_angle, previous_angle, offsets_valid;
    uint16_t unknown0a;
    uint32_t pulse_scale, positive_limit, negative_limit, interpolation_numerator;
    uint32_t unknown1c, unknown20, error, previous_error, stall_count, integral;
}
MotionFeedback;
_Static_assert(offsetof(MotionFeedback, pulse_scale)==0xc, "pulse scale");
_Static_assert(offsetof(MotionFeedback, error)==0x24, "error");
_Static_assert(offsetof(MotionFeedback, integral)==0x30, "integral");
_Static_assert(sizeof(MotionPosition)==40, "positions");
#define POSITION PTR(MotionPosition,WORK_RAM(WS_MOTION_POSITION))
#define FEEDBACK PTR(MotionFeedback,WORK_RAM(WS_MOTION_FEEDBACK))
#define CONTROL_OUTPUT RAM32(WORK_RAM(WS_CONTROL_OUTPUT))
extern uint64_t factory_multiply64(uint64_t, uint64_t);
extern uint64_t factory_scaled_position(uint64_t, uint32_t);
extern int32_t factory_signed_divide(int32_t, int32_t);
extern uint32_t factory_divide(uint32_t, uint32_t);
extern uint32_t abs_word(uint32_t);
extern uint32_t factory_read_encoder(void);
extern uint16_t encoder_lut_read(uint32_t);
extern void set_phase_and_value(uint32_t, uint32_t);
extern void fault_indicator_write(uint32_t), pause_control_chain(void), clear_timer_output_field(void), reset_compare_values(void);
static inline uint32_t round_q9(uint32_t raw) {
    if (!raw)return 0;
    uint32_t mag=raw>>31?0u-raw:raw;
    uint32_t q=(uint32_t)((int32_t)(mag+256u)>>9);
    return raw>>31?0u-q:q;
}
/* the factory's nonstandard 64-bit position scaling. Do not replace
 * with a mathematical division: it uses motor geometry dependent constants
 * and rounds the low word independently. */
uint64_t scale_position(uint64_t input, uint32_t divisor) {
    unsigned negative=(unsigned)(input>>63);
    uint64_t mag=negative?0-input:input;
    uint32_t lo=(uint32_t)mag, hi=(uint32_t)(mag>>32);
    uint64_t q=factory_divide(lo+(divisor>>1), divisor);
    if (hi) {
        unsigned shift=MOTOR->motor_geometry==50?3:4;
        uint32_t constant=shift==3?0x051eb852u:0x0a3d70a4u;
        q+=factory_multiply64(hi, constant);
        q-=factory_divide((hi<<shift)+50u, 100u);
    }
    return negative?0-q:q;
}
void motion_timer_irq(void) {
    uint16_t old_count=FEEDBACK->previous_pulse;
    uint32_t mode=LIVE->control_mode;
    if (mode!=2||RAM8(WORK_RAM(WS_UART_MOTION_STATE))==2||RAM8(WORK_RAM(WS_UART_MOTION_STATE))==1) {
        uint16_t count=(uint16_t)(mode==2?RAM16(WORK_RAM(WS_UART_PULSE_COUNTER)):RAM32(0x40003000u));
        POSITION->pulse_count+=(uint64_t)(int64_t)(int16_t)(count-old_count);
        FEEDBACK->previous_pulse=count;
    }
    uint64_t raw=factory_multiply64(POSITION->pulse_count, (uint64_t)(int64_t)(int32_t)FEEDBACK->pulse_scale);
    POSITION->target=factory_scaled_position(raw+(MOTOR->motor_geometry>>1), MOTOR->motor_geometry);
    if (!LIVE->interpolation||LIVE->microsteps>166) {
        POSITION->interpolated_target=POSITION->target;
    }
    else {
        uint32_t delta=(uint32_t)(POSITION->target-POSITION->interpolated_target);
        if ((int32_t)delta>2||(int32_t)delta< -2) {
            uint32_t magnitude=abs_word(delta);
            int32_t ratio=factory_signed_divide((int32_t)(FEEDBACK->interpolation_numerator+(uint32_t)((int32_t)magnitude>>1)), (int32_t)magnitude);
            if (ratio>=167)delta=(int32_t)delta>0?2u:0u-2u;
            else if ((int32_t)LIVE->microsteps<ratio)delta=(uint32_t)factory_signed_divide((int32_t)(delta*LIVE->microsteps), ratio);
        }
        POSITION->interpolated_target+=(uint64_t)(int64_t)(int32_t)delta;
    }
    if (!LIVE->control_mode) {
        uint32_t current=MOTOR->current_raw;
        if (current)current+=600;
        set_phase_and_value((uint16_t)(((uint16_t)POSITION->interpolated_target*MOTOR->motor_geometry+16u)>>5), current);
    }else {
        RAM32(0x40021014u)&=~0x80000u;
        FEEDBACK->encoder_angle=encoder_lut_read(factory_read_encoder());
        POSITION->encoder+=(uint64_t)(int64_t)(int16_t)(FEEDBACK->encoder_angle-(uint16_t)POSITION->encoder);
        if ((int32_t)abs_word((uint32_t)(POSITION->encoder-POSITION->previous_encoder))<30 && (int32_t)abs_word(CONTROL_OUTPUT)>4000) {
            FEEDBACK->stall_count++;
        }
        else {
            FEEDBACK->stall_count=0;
            fault_indicator_write(0);
        }
        POSITION->previous_encoder=POSITION->encoder;
        int64_t error=(int64_t)(POSITION->interpolated_target-POSITION->encoder);
        int32_t limit=LIVE->error_limit;
        if (error>limit)error=limit;
        else if (error< -limit)error= -limit;
        FEEDBACK->error=(uint32_t)error;
        if ((int32_t)FEEDBACK->stall_count>=1000) {
            FEEDBACK->stall_count=1000;
            FEEDBACK->stalled=1;
            if (LIVE->protection==1) {
                pause_control_chain();
                clear_timer_output_field();
                reset_compare_values();
            }
        }else if (FEEDBACK->stalled)FEEDBACK->stalled=0;
        if (!FEEDBACK->stalled||!LIVE->protection) {
            uint32_t proportional=round_q9(FEEDBACK->error*GAINS->kp);
            FEEDBACK->integral+=FEEDBACK->error*GAINS->ki;
            uint32_t integral=round_q9(FEEDBACK->integral);
            if ((int32_t)integral>(int32_t)FEEDBACK->positive_limit) {
                integral=FEEDBACK->positive_limit;
                FEEDBACK->integral=FEEDBACK->positive_limit<<9;
            }
            else if ((int32_t)integral<(int32_t)FEEDBACK->negative_limit) {
                integral=FEEDBACK->negative_limit;
                FEEDBACK->integral=0u-(FEEDBACK->positive_limit<<9);
            }
            uint32_t derivative=round_q9((FEEDBACK->error-FEEDBACK->previous_error)*GAINS->kd);
            int32_t output=(int32_t)(proportional+integral+derivative);
            if (output>8000)output=8000;
            else if (output< -8000)output= -8000;
            CONTROL_OUTPUT=(uint32_t)output;
            FEEDBACK->previous_error=FEEDBACK->error;
        }
        RAM32(0x40021014u)|=0x80000u;
    }
    RAM32(0x40005814u)=0xfeu;
}
