#include "factory_layout.h"
/* Auxiliary timer interrupt: UART motion progression, completion reporting
 * and display sleep counter. Preserve signed 64-bit comparisons and word
 * wrapping. */
extern uint32_t timer_test_flag(uint32_t, uint32_t), timer_clear_flag(uint32_t, uint32_t);
extern void uart_start_reply(void);
static uint64_t pair(uint32_t a) {
    uint32_t l=RAM32(a), h=RAM32(a+4);
    return (uint64_t)l|((uint64_t)h<<32);
}
void auxiliary_timer_irq(void) {
    const uint32_t state=WORK_RAM(WS_UART_MOTION_STATE), target=WORK_RAM(WS_UART_TARGET_LOW), actual=WORK_RAM(WS_MOTION_POSITION), count=WORK_RAM(0x18), position=WORK_RAM(WS_UART_PULSE_COUNTER), step=WORK_RAM(WS_UART_MOTION_SPEED), direction=WORK_RAM(WS_UART_MOTION_DIRECTION), buttons=WORK_RAM(0x5dc);
    if (timer_test_flag(2, 0)==1) {
        if (RAM8(state)==2 && pair(actual)==pair(target)) {
            RAM32(count)++;
            if (RAM32(count)==25) {
                UART->tx_reverse[1]=2;
                UART->tx_reverse[0]=(uint8_t)(LIVE->uart_address+2);
                UART->tx_remaining=2;
                uart_start_reply();
            }
            if (RAM32(count)>=50)RAM8(state)=0;
        }
        if (LIVE->control_mode==2) {
            uint8_t mode=RAM8(state);
            if (mode==1) {
                uint16_t n=RAM16(step);
                if (RAM8(direction))RAM16(position)+=n;
                else RAM16(position)-=n;
            }
            else if (mode==2 && pair(actual)!=pair(target)) {
                RAM32(count)=0;
                uint64_t a=pair(actual), t=pair(target);
                uint16_t n=RAM16(step);
                if (RAM8(direction)) {
                    if ((int64_t)t >= (int64_t)(a+n))RAM16(position)+=n;
                    else RAM16(position)+=(uint16_t)(t-a);
                }
                else {
                    if ((int64_t)(a-n) >= (int64_t)t)RAM16(position)-=n;
                    else RAM16(position)-=(uint16_t)(a-t);
                }
            }
        }else if (LIVE->autosdd==1 && RAM32(WORK_RAM(WS_MENU_TABLE_ADDRESS))==WORK_RAM(0x358)) {
            RAM16(buttons+4)++;
            if (RAM16(buttons+4)>4096)RAM16(buttons+4)=4096;
        }else RAM16(buttons+4)=0;
    }
    (void)timer_clear_flag(2, 0);
}
