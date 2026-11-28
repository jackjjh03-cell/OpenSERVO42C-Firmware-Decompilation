#include "settings_flash.h"
static uint32_t read_reg(void *ctx,uint32_t a){(void)ctx;return *(volatile uint32_t *)(uintptr_t)a;}
static void write_reg(void *ctx,uint32_t a,uint32_t v){(void)ctx;*(volatile uint32_t *)(uintptr_t)a=v;}
static uint8_t read_byte(void *ctx,uint32_t a){(void)ctx;return *(volatile uint8_t *)(uintptr_t)a;}
static void erase_trigger(void *ctx,uint32_t a){(void)ctx;*(volatile uint8_t *)(uintptr_t)a=0;}
static void program_halfword(void *ctx,uint32_t a,uint16_t v){(void)ctx;*(volatile uint16_t *)(uintptr_t)a=v;}
os_flash os_hc32_settings_flash(void){return (os_flash){0,read_reg,write_reg,read_byte,erase_trigger,program_halfword,false};}
