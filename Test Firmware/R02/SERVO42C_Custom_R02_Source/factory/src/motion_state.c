#include "factory_layout.h"
/* Transitional fixed RAM fields; physical units still unresolved. */
void clear_feedback_state(void) {
    RAM8(WORK_RAM(WS_STARTUP_FEEDBACK_READY))=0;
    RAM32(WORK_RAM(WS_STARTUP_FEEDBACK_SUM_2))=0;
    RAM32(WORK_RAM(WS_STARTUP_FEEDBACK_SUM_1))=0;
    RAM16(WORK_RAM(WS_STARTUP_FEEDBACK_COUNT))=0;
}
void set_phase_and_value(uint32_t phase, uint32_t value) {
    RAM16(WORK_RAM(WS_PHASE_INDEX))=phase&1023u;
    RAM32(WORK_RAM(WS_CURRENT_COMMAND_AXIS_A))=value;
}
void feedback_difference(uint32_t out) {
    uint32_t a=RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_2));
    a-=RAM32(WORK_RAM(WS_ADC_FEEDBACK_2_RAW));
    uint32_t b=RAM32(WORK_RAM(WS_ADC_FEEDBACK_1_RAW));
    b-=RAM32(WORK_RAM(WS_ADC_OFFSET_CHANNEL_1));
    RAM32(out)=a;
    RAM32(out+4)=b;
}
