#include "factory_layout.h"
extern void render_menu_rows(void);
extern uint32_t gpio_read_input_bit(uint32_t, uint32_t);
/* Two-button edge detection, menu selection and callback dispatch. Menu
 * descriptors reside in the workspace. */
#define MENU_BASE WORK_RAM(WS_MENU_STATE)
#define BUTTON_BASE WORK_RAM(0x05dcu)
void menu_poll_buttons(void) {
    render_menu_rows();
    if (gpio_read_input_bit(0, 9)==0 && !(RAM8(BUTTON_BASE+1)&1u)) {
        RAM8(BUTTON_BASE+1)|=1;
        RAM16(BUTTON_BASE+4)=1;
        uint32_t sub=RAM32(MENU_BASE+8);
        if (sub) {
            uint8_t i=++RAM8(MENU_BASE+1);
            if (!RAM8(sub+9u*i))RAM8(MENU_BASE+1)=0;
        }
        else {
            uint8_t i=++RAM8(MENU_BASE);
            if (!RAM8(RAM32(MENU_BASE+4)+20u*i))RAM8(MENU_BASE)=0;
        }
    }
    if (gpio_read_input_bit(0, 10)==0 && !(RAM8(BUTTON_BASE+1)&2u)) {
        RAM8(BUTTON_BASE+1)|=2;
        RAM16(BUTTON_BASE+4)=0;
        uint32_t row=RAM32(MENU_BASE+4)+20u*RAM8(MENU_BASE);
        typedef uint32_t (*Callback)(uint32_t);
        if (RAM32(MENU_BASE+8)) {
            ((Callback)(uintptr_t)RAM32(row+12))(RAM8(MENU_BASE+1));
            RAM32(MENU_BASE+8)=0;
        }else {
            uint32_t sub=RAM32(row+16);
            if (sub) {
                RAM32(MENU_BASE+8)=sub;
                RAM8(MENU_BASE+1)=(uint8_t)((Callback)(uintptr_t)RAM32(row+12))(255);
            }
            else ((Callback)(uintptr_t)RAM32(row+12))(255);
        }
    }
    if (gpio_read_input_bit(0, 9)==1)RAM8(BUTTON_BASE+1)&=(uint8_t)~1u;
    if (gpio_read_input_bit(0, 10)==1)RAM8(BUTTON_BASE+1)&=(uint8_t)~2u;
}
