#include "board.h"
#define REG(a) (*(volatile uint32_t *)(uintptr_t)(a))
static uint32_t read_reg(void *ctx,uint32_t addr){(void)ctx;return REG(addr);}
static void write_reg(void *ctx,uint32_t addr,uint32_t value){(void)ctx;REG(addr)=value;}
static uint16_t read_trim(void *ctx,uint32_t addr){(void)ctx;return *(volatile uint16_t *)(uintptr_t)addr;}
/* Bounded conservative startup delay. Core runs at no more than 48 MHz;
 * exact runtime timing is supplied by the target scheduler, not this loop. */
static void delay_us(void *ctx,uint32_t us){(void)ctx;while(us--)for(unsigned i=0;i<48u;i++)__asm volatile("nop");}
os_board_io os_hc32_board_io(void){return (os_board_io){0,read_reg,write_reg,read_trim,delay_us};}
