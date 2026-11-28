#include "feedback_dma.h"
#include "bringup.h"
#include <assert.h>
#include <stdio.h>
typedef struct {uint32_t a,v;} cell;
typedef struct {cell cells[48];unsigned n;} fake;
static uint32_t read(void *p,uint32_t a){fake *f=p;for(unsigned i=0;i<f->n;i++)if(f->cells[i].a==a)return f->cells[i].v;return 0;}
static void write(void *p,uint32_t a,uint32_t v){fake *f=p;if(a==0x40000c30u||a==0x40000d30u)assert(!(v&0x1000u));for(unsigned i=0;i<f->n;i++)if(f->cells[i].a==a){f->cells[i].v=v;return;}assert(f->n<48);f->cells[f->n++]=(cell){a,v};}
int main(void){fake f={0};os_board_io io={&f,read,write,0,0};
    os_feedback_dma_start(&io,0x20000100u);
    assert(read(&f,0x40021018u)==0x40002490u&&read(&f,0x4002101cu)==0x20000100u);
    assert(read(&f,0x40021010u)==0x92020000u&&read(&f,0x40021014u)==0x08e80001u);
    assert(read(&f,0x40000c20u)==0x81u&&read(&f,0x40000c0cu)==1u);
    write(&f,0x40021014u,read(&f,0x40021014u)|0x50000u);
    assert(os_feedback_dma_complete(&io));assert(!os_feedback_dma_complete(&io));
    os_feedback_dma_stop(&io);assert(!(read(&f,0x40021014u)&0x80000u));
    os_controller c;os_init(&c);os_bench b={.ms=0x12345678,.core_hz=48000000,.adc_samples=99,.raw_angle=16383,.offset1=2048,.offset2=2000,.flags=15};
    uint8_t in[8]={0xa5,0x5a,1,17,12,0,0,0},out[56];uint16_t crc=os_crc16(in+2,4);in[6]=(uint8_t)crc;in[7]=(uint8_t)(crc>>8);
    assert(os_bench_dispatch(&c,&b,in,8,out)==56);assert(out[3]==17&&out[4]==0x8c&&out[5]==48&&out[7]==0x78&&out[10]==0x12&&out[47]==0&&out[48]==8&&out[53]==15);
    assert(os_crc16(out+2,52)==(uint16_t)(out[54]|((uint16_t)out[55]<<8)));
    in[6]^=1;assert(os_bench_dispatch(&c,&b,in,8,out)==0);
    puts("feedback DMA and diagnostics passed");return 0;
}
