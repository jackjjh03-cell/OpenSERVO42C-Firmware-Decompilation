#include "firmware_api.h"
extern void factory_gpio_set(uint32_t, uint32_t), factory_gpio_clear(uint32_t, uint32_t), factory_delay_ms(uint32_t);
extern void factory_oled_command(uint32_t);
extern void factory_pause_control(void), factory_flash_erase(uint32_t);
extern void factory_flash_halfword(uint32_t, uint32_t);
/* preserve exact initialization command sequence and delays. */
void oled_initialize(void) {
    factory_gpio_set(0, 12);
    factory_delay_ms(100);
    factory_gpio_clear(0, 12);
    factory_delay_ms(100);
    static const uint8_t commands[]={0xae, 0xd5, 0xf0, 0xa8, 0x27, 0xd3, 0, 0x40, 0x8d, 0x14, 0x20, 2, 0xa1, 200, 0xda, 0x12, 0xad, 0x30, 0x81, 0x40, 0xd9, 0x22, 0xdb, 0x20, 0xa4, 0xa6, 0xc, 0x11, 0xaf};
    for (unsigned i=0;i<sizeof(commands);i++)factory_oled_command(commands[i]);
    oled_flush_all();
}
void menu_request_redraw(void) {
    RAM8(WORK_RAM(WS_MENU_REDRAW_FLAG))=1;
}
void menu_select_table(uint32_t table) {
    RAM32(WORK_RAM(WS_MENU_TABLE_ADDRESS))=table;
    RAM8(WORK_RAM(WS_MENU_STATE))=0;
}
/* Restore defaults, commit settings and report completion on the OLED. */
void factory_restore(void) {
    factory_pause_control();
    factory_flash_erase(SETTINGS_PAGE);
    load_or_initialize_settings();
    factory_commit();
    display_three_lines(TEXT_ADDR(msg_restore_reload), TEXT_ADDR(msg_restore_ok), TEXT_ADDR(msg_restore_blank));
    factory_delay_ms(1200);
}
/* Program consecutive flash halfwords. Count is in halfwords, not bytes. */
__attribute__((noinline)) void factory_flash_write(uint32_t dest, uint32_t src, uint32_t count) {
    for (uint32_t i=0;i<count;i=(i+1u)&0xffffu) {
        factory_flash_halfword(dest, RAM16(src));
        dest+=2;
        src+=2;
    }
}
void factory_flash_erase_write(uint32_t dest, uint32_t src, uint32_t count) {
    factory_flash_erase(dest);
    factory_flash_write(dest, src, count);
}
