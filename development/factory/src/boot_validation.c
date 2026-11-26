#include "factory_layout.h"
extern void start_feedback_capture(void), factory_delay_ms(uint32_t), display_three_lines(uint32_t, uint32_t, uint32_t);
/* Wait for startup power and feedback thresholds. This sequence blocks until
 * the expected conditions are met. */
uint32_t wait_for_startup_feedback(void) {
    start_feedback_capture();
    factory_delay_ms(3);
    while (!RAM8(WORK_RAM(WS_STARTUP_FEEDBACK_READY))) {
        if (RAM16(WORK_RAM(WS_STARTUP_FEEDBACK_COUNT))==65535) {
            display_three_lines(TEXT_ADDR(msg_power_waiting), TEXT_ADDR(msg_power_v), TEXT_ADDR(msg_power_power));
            while (!RAM8(WORK_RAM(WS_STARTUP_FEEDBACK_READY))) {
            }
        }
    }
    return (int32_t)RAM32(WORK_RAM(WS_STARTUP_FEEDBACK_SUM_2))>35 && (int32_t)RAM32(WORK_RAM(WS_STARTUP_FEEDBACK_SUM_1))>35;
}
