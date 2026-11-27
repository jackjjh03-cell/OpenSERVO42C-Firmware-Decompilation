#include "factory_layout.h"
extern uint32_t gpio_set_output_bit(uint32_t, uint32_t), adc_startup_config(uint32_t), adc_config_sequence(uint32_t), adc_select_channel(uint32_t, uint32_t);
extern void adc_enable(void), adc_power_on(void), factory_delay_ms(uint32_t), adc_clear_flags(uint32_t), clock_adc_stop(void), clear_feedback_state(void), adc_startup_irq_enable(void);
/* all original ADC input setup, clock and channel choices. */
void initialize_adc(void) {
    gpio_set_output_bit(64, 0);
    gpio_set_output_bit(64, 1);
    gpio_set_output_bit(64, 11);
    adc_enable();
    adc_power_on();
    factory_delay_ms(1);
    uint16_t cfg[6]={0x0801, 0x2000, 0x0600, 0, 0, 0};
    adc_startup_config((uint32_t)(uintptr_t)cfg);
    uint8_t seq[2]={3, 0};
    adc_config_sequence((uint32_t)(uintptr_t)seq);
    adc_select_channel(0, 8);
    adc_select_channel(1, 9);
    adc_select_channel(2, 18);
    adc_clear_flags(32);
}
/* startup feedback capture drive sequence. Preserve original compare values. */
void start_feedback_capture(void) {
    clock_adc_stop();
    clear_feedback_state();
    RAM32(0x40000c3cu)=(RAM32(0x40000c3cu)&0xffff0000u)|1200u;
    RAM32(0x40000c40u)=(RAM32(0x40000c40u)&0xffff0000u)|900u;
    RAM32(0x40000d3cu)=(RAM32(0x40000d3cu)&0xffff0000u)|900u;
    RAM32(0x40000d40u)=(RAM32(0x40000d40u)&0xffff0000u)|1200u;
    uint8_t seq[2]={3, 0};
    adc_config_sequence((uint32_t)(uintptr_t)seq);
    adc_startup_irq_enable();
    RAM32(0x40000c0cu)|=1u;
}
extern uint32_t peripheral_clock_frequency(void), uart_configure(uint32_t, uint32_t);
extern void uart_disable(void), factory_uart_tx_end(void), factory_irq_config(uint32_t, uint32_t, uint32_t);
/* original UART enable sequence, no new baud assumptions. */
extern volatile uint8_t magnet_lock;
void factory_apply_uart(void) {
    if (magnet_lock || !LIVE->uart_baud) {
        uart_disable();
        return;
    }
    uint32_t cfg[4];
    cfg[0]=64;
    cfg[1]=512;
    cfg[2]=peripheral_clock_frequency();
    cfg[3]=LIVE->uart_baud;
    uart_configure(0x40000000u, (uint32_t)(uintptr_t)cfg);
    RAM32(0x40020c34u)=(RAM32(0x40020c34u)&~7u)|2u;
    RAM32(0x40020c38u)&=~7u;
    RAM32(0x40020d00u)|=0x4000u;
    factory_uart_tx_end();
    factory_irq_config(6, 1, 1);
}
/* OLED bus transmit state machine. Original polling limit, error
 * flag, address, control sequence, byte-counter wrap and STOP behavior. */
void factory_oled_write(uint32_t mode, uint32_t data, uint32_t count) {
    const uint32_t ctrl=0x40000408u, status=0x40000414u, tx=0x4000040cu;
    uint8_t i=0;
    RAM32(ctrl)|=32;
    for (;;) {
        unsigned retry=0;
        while (!(RAM32(ctrl)&8u)) {
            if (++retry>6000) {
                RAM16(WORK_RAM(0x05deu))=10;
                i=(uint8_t)(count+1);
                break;
            }
        }
        switch (RAM32(status)) {
        case 8: RAM32(ctrl)&=~32u;
            RAM32(tx)=120;
            break;
        case 24: RAM32(tx)=mode;
            break;
        case 40: RAM32(tx)=RAM8(data+i);
            i++;
            break;
        case 32:case 56: RAM32(ctrl)|=32u;
            break;
        case 48:RAM32(ctrl)|=16u;
            break;
        default:RAM16(WORK_RAM(0x05deu))=10;
            i=(uint8_t)(count+1);
            break;
        }
        if (i>count) {
            RAM32(ctrl)|=16u;
            RAM32(ctrl)&=~8u;
            return;
        }
        RAM32(ctrl)&=~8u;
    }
}
void factory_oled_command(uint32_t v) {
    factory_oled_write(0, (uint32_t)(uintptr_t)&v, 1);
}
