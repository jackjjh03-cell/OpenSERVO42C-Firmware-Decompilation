#include "board.h"
#include <assert.h>
#include <stdio.h>
typedef struct {uint32_t addr,val;} cell;
typedef struct {cell regs[256];unsigned n,conversions;bool lock,adc,button,bad_offset;} fake;
static uint32_t read(void *ctx,uint32_t addr){fake *f=ctx;
    if(addr==0x4000203cu&&f->lock){for(unsigned i=0;i<f->n;i++)if(f->regs[i].addr==addr)return f->regs[i].val|0x40000u;return 0x40000u;}
    if(addr==0x400024b0u)return f->adc?0x20u:0;
    if(addr==0x40002498u)return f->bad_offset?1000u:2048u;
    if(addr==0x40002494u)return 2000u;
    if(addr==0x40020d04u)return f->button?512u:0;
    for(unsigned i=0;i<f->n;i++)if(f->regs[i].addr==addr)return f->regs[i].val;
    return 0;
}
static void write(void *ctx,uint32_t addr,uint32_t v){fake *f=ctx;
    assert(addr>=0x40000000u); /* no flash or identity writes */
    if(addr==0x40000c30u||addr==0x40000d30u)assert(!(v&0x1000u));
    if(addr==0x400024c8u&&v==1u)f->conversions++;
    for(unsigned i=0;i<f->n;i++)if(f->regs[i].addr==addr){f->regs[i].val=v;return;}
    assert(f->n<256);f->regs[f->n++]=(cell){addr,v};
}
static uint16_t trim(void *ctx,uint32_t addr){(void)ctx;return (uint16_t)addr;}
static void delay(void *ctx,uint32_t us){(void)ctx;assert(us<=5000u);}
int main(void){fake f={.lock=true,.adc=true,.button=true};os_board_io io={&f,read,write,trim,delay};os_offsets o;
    assert(os_board_prepare(&io,&o)==OS_BOARD_OK);assert(o.slot2==2048&&o.slot1==2000&&f.conversions==18);
    assert((read(&f,0x40000c3cu)&65535u)==1200);assert((read(&f,0x40000d40u)&65535u)==1200);
    assert((read(&f,0x4000244cu)&0x7fffu)==(8u|(9u<<5)|(18u<<10)));
    assert(read(&f,0x40002004u)&256u);
    assert((read(&f,0x40020d00u)&0x6000u)==0x6000u);
    assert((read(&f,0x40020d1cu)&0x6000u)==0x6000u);
    assert(read(&f,0x40020c34u)==0&&read(&f,0x40020c38u)==0);
    os_board_step_init(&io,false);assert(read(&f,0x40003088u)==16&&read(&f,0x4000308cu)==64);
    os_board_step_init(&io,true);assert(read(&f,0x40003088u)==64&&read(&f,0x4000308cu)==16);
    write(&f,0x40020e50u,0xffffu);os_board_enable_irq_init(&io);
    assert(read(&f,0x40020e50u)==0xff7fu);
    assert(read(&f,0x40020d78u)&128u);assert(read(&f,0x40020d7cu)&128u);
    assert(!os_board_enable_irq_take(&io));
    write(&f,0x40020e40u,128u);assert(os_board_enable_irq_take(&io));
    f=(fake){0};assert(os_board_prepare(&io,&o)==OS_BOARD_CLOCK);
    f=(fake){.lock=true};assert(os_board_prepare(&io,&o)==OS_BOARD_BUTTON);
    f=(fake){.lock=true,.button=true};assert(os_board_prepare(&io,&o)==OS_BOARD_ADC);
    f=(fake){.lock=true,.button=true,.adc=true,.bad_offset=true};assert(os_board_prepare(&io,&o)==OS_BOARD_OFFSET);
    puts("board startup tests passed");return 0;
}
