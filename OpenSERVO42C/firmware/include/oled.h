#ifndef OS_OLED_H
#define OS_OLED_H
#include "board.h"
/* Stock display: 72 x 40 pixels, five pages, column offset 28. */
typedef struct {
    int32_t position;
    uint32_t faults,adc_errors,encoder_errors;
    uint16_t load,raw_angle;
    uint8_t state,mode,board_status,flags;
} os_oled_view;
/* flags: bit0 reference, bit1 PWM gate, bit2 interpreted enable. */
bool os_oled_init(const os_board_io *io,uint32_t peripheral_hz);
bool os_oled_page(const os_board_io *io,unsigned page,const uint8_t pixels[72]);
void os_oled_render(const os_oled_view *v,bool diagnostics,unsigned page,uint8_t pixels[72]);
/* Two equal consecutive samples qualify a button press. No motor actions. */
typedef struct {uint8_t previous,stable;bool diagnostics;} os_oled_buttons;
void os_oled_button_sample(os_oled_buttons *b,uint8_t pressed);
void os_board_oled_pins(const os_board_io *io);
#endif
