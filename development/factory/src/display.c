#include "firmware_api.h"
/* Three 7 x 14 text rows occupy five 72-byte OLED pages. Use supported
 * glyphs and at most 10 characters per row. Glyph data is defined in
 * layout_assets.c. */
#define DISPLAY_BUFFER WORK_RAM(0x08ecu)
#define ROW_WIDTHS PTR(uint16_t,WORK_RAM(0x0622u))
#define ROW_LENGTHS PTR(uint8_t,WORK_RAM(0x0008u))
#define FONT_BASE ASSET_ADDR(oled_font, 0x0u)
extern void factory_oled_write(uint32_t control, uint32_t data, uint32_t size);
extern void factory_oled_command(uint32_t command);
extern uint32_t factory_strlen(uint32_t string);
void oled_set_position(uint32_t column, uint32_t page) {
    factory_oled_command((uint8_t)(page+0xB0));
    factory_oled_command((column&15u)+12u);
    factory_oled_command((column>>4)+17u);
}
void oled_flush_all(void) {
    for (unsigned page=0;page<5;page++) {
        oled_set_position(0, page);
        factory_oled_write(0x40, DISPLAY_BUFFER+72u*page, 72);
    }
}
void oled_clear_old_text(uint32_t line0, uint32_t line1, uint32_t line2) {
    uint8_t len[3];
    len[0]=(uint8_t)factory_strlen(line0);
    len[1]=(uint8_t)factory_strlen(line1);
    len[2]=(uint8_t)factory_strlen(line2);
    for (unsigned row=0;row<3;row++) {
        unsigned old=ROW_LENGTHS[row], now=len[row];
        ROW_WIDTHS[row]=(uint16_t)(7u*(old>now?old:now));
        for (unsigned col=7u*now;col<7u*old;col++) {
            uint32_t a=DISPLAY_BUFFER+col;
            if (row==0) {
                RAM8(a)=0;
                RAM8(a+72)&=0xC0;
            }
            else if (row==1) {
                RAM8(a+72)&=0x3F;
                RAM8(a+144)=0;
                RAM8(a+216)&=0xF0;
            }
            else {
                RAM8(a+216)&=0x0F;
                RAM8(a+288)=0;
            }
        }
    }
    ROW_LENGTHS[0]=len[0];
    ROW_LENGTHS[1]=len[1];
    ROW_LENGTHS[2]=len[2];
}
void oled_render_text(uint32_t line0, uint32_t line1, uint32_t line2) {
    uint32_t lines[3]={line0, line1, line2};
    for (unsigned row=0;row<3;row++) {
        uint32_t line=lines[row];
        for (unsigned x=0;RAM8(line+x);x++) {
            unsigned glyph=(uint16_t)(RAM8(line+x)-0x20u);
            uint32_t font=FONT_BASE+14u*glyph;
            for (unsigned k=0;k<7;k++) {
                uint32_t a=DISPLAY_BUFFER+7u*x+k;
                uint8_t lo=RAM8(font+k), hi=RAM8(font+k+7);
                if (row==0) {
                    RAM8(a)=lo;
                    RAM8(a+72)=hi;
                }
                else if (row==1) {
                    RAM8(a+72)=(uint8_t)((RAM8(a+72)&0x3F)|(lo<<6));
                    RAM8(a+144)=(uint8_t)((lo>>2)|(hi<<6));
                    RAM8(a+216)=(uint8_t)((RAM8(a+216)&0xF0)|(hi>>2));
                }
                else {
                    RAM8(a+216)=(uint8_t)((RAM8(a+216)&0x0F)|(lo<<4));
                    RAM8(a+288)=(uint8_t)((lo>>4)|(hi<<4));
                }
            }
        }
    }
}
void display_three_lines(uint32_t line0, uint32_t line1, uint32_t line2) {
    oled_clear_old_text(line0, line1, line2);
    oled_render_text(line0, line1, line2);
    for (unsigned page=0;page<5;page++) {
        unsigned row=page>>1, width=ROW_WIDTHS[row];
        if (page&1u) {
            unsigned adjacent=ROW_WIDTHS[row+1];
            if (adjacent>width)width=adjacent;
        }
        oled_set_position(0, page);
        factory_oled_write(0x40, DISPLAY_BUFFER+72u*page, width);
    }
}
