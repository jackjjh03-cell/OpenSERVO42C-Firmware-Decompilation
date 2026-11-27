#include "firmware_api.h"
/* Caller supplies a 14-bit raw encoder reading; no mask in factory code. */
uint16_t encoder_lut_read(uint32_t raw) {
    if (!RAM8(WORK_RAM(WS_ENCODER_LUT_READY))) return 0;
    return *PTR(uint16_t, CALIBRATION_LUT_BASE+raw*2u);
}

/* preserve board-specific knots and flags; no flash writes. */
uint32_t load_calibration_knots(void) {
    if (RAM16(CALIBRATION_KNOT_VALID)!=1||RAM16(CALIBRATION_LUT_VALID)!=1)return 0;
    RAM16(WORK_RAM(WS_CALIBRATION_META_RAW))=RAM16(CALIBRATION_KNOT_META);
    RAM16(WORK_RAM(WS_CALIBRATION_WRAP_THRESHOLD))=RAM16((CALIBRATION_KNOT_META+2u));
    for (unsigned i=0;i<200;i++)RAM16(WORK_RAM(WS_CALIBRATION_KNOTS)+2u*i)=RAM16(CALIBRATION_KNOT_PAGE+2u*i);
    RAM16(WORK_RAM(WS_CALIBRATION_KNOT_VALID))=1;
    RAM16(WORK_RAM(WS_CALIBRATION_LUT_VALID))=1;
    RAM8(WORK_RAM(WS_ENCODER_LUT_READY))=1;
    return 1;
}
extern uint32_t factory_read_encoder(void);
/* multi-turn accumulator. Keep strict half-turn ties and word wrap. */
void update_multiturn_angle(void) {
    uint32_t angle;
    if (RAM8(WORK_RAM(WS_DRIVE_ENABLE_STATE))==0)angle=encoder_lut_read(factory_read_encoder());
    else angle=RAM16(WORK_RAM(WS_ENCODER_ANGLE));
    uint32_t low=RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_LOW));
    uint32_t high=RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_HIGH));
    int32_t delta=(int32_t)angle-(int32_t)(low&65535u);
    if (delta>32768) {
        low=RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_LOW));
        high=RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_HIGH));
        high-=low<65536u;
        low-=65536u;
        RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_LOW))=low;
        RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_HIGH))=high;
    }
    if (delta< -32768) {
        low=RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_LOW));
        high=RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_HIGH));
        high+=low>=0xffff0000u;
        low+=65536u;
        RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_LOW))=low;
        RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_HIGH))=high;
    }
    low=RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_LOW));
    high=RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_HIGH));
    RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_LOW))=(low&0xffff0000u)|angle;
    RAM32(WORK_RAM(WS_ENCODER_ACCUMULATOR_HIGH))=high;
}
extern int32_t factory_signed_divide(int32_t, int32_t);
extern uint32_t factory_divide(uint32_t, uint32_t);
extern void display_three_lines(uint32_t, uint32_t, uint32_t);
/* interpolation. Preserve terminal error, signed rounding and 16-bit wrap. */
uint32_t calibration_interpolate(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x) {
    uint32_t dx=x1-x0;
    if (!dx) {
        display_three_lines(TEXT_ADDR(msg_interpolation_dx_0), TEXT_ADDR(msg_interpolation_reboot), TEXT_ADDR(msg_interpolation_error));
        for (;;) {
        }
    }
    uint32_t numerator=(y1-y0)*(x-x0)+(uint32_t)((int32_t)dx>>1);
    return (uint16_t)((uint32_t)factory_signed_divide((int32_t)numerator, (int32_t)dx)+y0);
}
/* inverse map from the 200 calibration knots to the 16-bit angle. */
uint32_t encoder_inverse_lookup(int32_t input) {
    if (input<(int32_t)RAM16(WORK_RAM(WS_CALIBRATION_WRAP_THRESHOLD)))input+=16384;
    for (unsigned i=0;i<200;i++) {
        uint32_t a=RAM16(WORK_RAM(WS_CALIBRATION_KNOTS)+2u*i), b=RAM16(WORK_RAM(WS_CALIBRATION_KNOTS)+2u*(i==199?0:i+1));
        if ((a>b?a-b:b-a)>8192) {
            if (a<b)a+=16384;
            else b+=16384;
        }
        if (((int32_t)a<=input&&input<=(int32_t)b)||((int32_t)b<=input&&input<=(int32_t)a)) {
            uint32_t y0=factory_divide(i*8192u+12u, 25u), y1=factory_divide((i+1)*8192u+12u, 25u);
            return calibration_interpolate(a, y0, b, y1, (uint32_t)input);
        }
    }
    display_three_lines(TEXT_ADDR(msg_lookup_reverse), TEXT_ADDR(msg_lookup_lookup), TEXT_ADDR(msg_lookup_error));
    for (;;) {
    }
}
