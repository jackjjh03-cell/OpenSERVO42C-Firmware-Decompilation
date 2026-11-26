#include "factory_layout.h"
/* Reproduce 32-bit wrapping arithmetic, including the factory's signed
 * rounding on overflow. ARM clang uses arithmetic signed right shifts. */
static __attribute__((always_inline)) inline uint32_t rounded_signed(uint32_t x, unsigned shift) {
    if (!x)return 0;
    uint32_t neg=0u-(x>>31), mag=(x^neg)-neg;
    uint32_t q=(uint32_t)((int32_t)(mag+(1u<<(shift-1)))>>shift);
    return (q^neg)-neg;
}
static __attribute__((always_inline)) inline int32_t scaled_difference(uint32_t a, uint32_t b) {
    int32_t q=(int32_t)rounded_signed((a-b)*RAM16(WORK_RAM(WS_EXTRA_SETTINGS)), 11);
    if (q>=(int32_t)RAM32(WORK_RAM(WS_CURRENT_POSITIVE_LIMIT)))return (int32_t)RAM32(WORK_RAM(WS_CURRENT_POSITIVE_LIMIT));
    if (q<=(int32_t)RAM32(WORK_RAM(WS_CURRENT_NEGATIVE_LIMIT)))return (int32_t)RAM32(WORK_RAM(WS_CURRENT_NEGATIVE_LIMIT));
    return q;
}
int32_t scaled_difference_a(uint32_t a, uint32_t b) {
    return scaled_difference(a, b);
}
int32_t scaled_difference_b(uint32_t a, uint32_t b) {
    return scaled_difference(a, b);
}
/* two-axis Q14 rotation, uses the recovered 1280-entry table.
 * Valid phase index 0..1023. Physical axis interpretation is inferred. */
void rotate_q14(uint32_t out, uint32_t a, uint32_t b, uint32_t phase) {
    RAM32(WORK_RAM(WS_ROTATION_SIN_Q14))=(uint32_t)(int32_t)(int16_t)RAM16(ASSET_ADDR(sine_q14, 0x0u)+phase*2u);
    RAM32(WORK_RAM(WS_ROTATION_COS_Q14))=(uint32_t)(int32_t)(int16_t)RAM16(ASSET_ADDR(sine_q14, 0x0u)+(phase+256u)*2u);
    uint32_t x=RAM32(WORK_RAM(WS_ROTATION_SIN_Q14))*b+RAM32(WORK_RAM(WS_ROTATION_COS_Q14))*a;
    uint32_t y=RAM32(WORK_RAM(WS_ROTATION_COS_Q14))*b-RAM32(WORK_RAM(WS_ROTATION_SIN_Q14))*a;
    RAM32(out)=rounded_signed(x, 14);
    RAM32(out+4)=rounded_signed(y, 14);
}
/* inverse Q14 rotation uses previously loaded sine/cosine. */
void inverse_rotate_q14(uint32_t out, uint32_t a, uint32_t b) {
    uint32_t x=RAM32(WORK_RAM(WS_ROTATION_COS_Q14))*a-RAM32(WORK_RAM(WS_ROTATION_SIN_Q14))*b;
    uint32_t y=RAM32(WORK_RAM(WS_ROTATION_SIN_Q14))*a+RAM32(WORK_RAM(WS_ROTATION_COS_Q14))*b;
    RAM32(out)=rounded_signed(x, 14);
    RAM32(out+4)=rounded_signed(y, 14);
}
