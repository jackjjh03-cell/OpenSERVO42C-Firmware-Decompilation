#ifndef FIRMWARE_API_H
#define FIRMWARE_API_H
#include "factory_layout.h"
/* Menu callback API: 255 queries the current value; other values select an
 * option. Call with a valid option index. */
uint32_t menu_control_mode(uint32_t option);
uint32_t menu_acceleration(uint32_t option);
uint32_t menu_uart_baud(uint32_t option);
uint32_t menu_current(uint32_t option);
uint32_t menu_autosdd(uint32_t option);
uint32_t menu_direction(uint32_t option);
uint32_t menu_enable(uint32_t option);
uint32_t menu_interpolation(uint32_t option);
uint32_t menu_microsteps(uint32_t option);
uint32_t menu_motor_type(uint32_t option);
uint32_t menu_uart_address(uint32_t option);
uint32_t menu_origin_direction(uint32_t option);
uint32_t menu_origin_speed(uint32_t option);
uint32_t menu_origin_mode(uint32_t option);
uint32_t menu_protection(uint32_t option);
void menu_exit(void);
void uart_reset_rx(void);
void uart_receive_byte(volatile UartState *state,uint16_t address,uint8_t byte);
void uart0_irq(void);
uint16_t encoder_lut_read(uint32_t raw);
void uart_dispatch(void);
void uart_start_reply(void);
void read_active_settings_record(void);
void select_settings_record(void);
uint32_t settings_slot_blank(void);
void load_or_initialize_settings(void);
void oled_set_position(uint32_t column,uint32_t page);
void oled_flush_all(void);
void oled_clear_old_text(uint32_t line0,uint32_t line1,uint32_t line2);
void oled_render_text(uint32_t line0,uint32_t line1,uint32_t line2);
void display_three_lines(uint32_t line0,uint32_t line1,uint32_t line2);
#endif
