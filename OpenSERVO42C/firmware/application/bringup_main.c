/* First target application: acquisition/communications only, never energizes
 * the bridge. Not a motor-drive release. No identity/settings/LUT writes. */
#include "board.h"
#include "bringup.h"
#include "encoder.h"
#include "phase.h"
#include "feedback_dma.h"
#include "hc32_inhibit.h"
#include "uart_transport.h"
#define REG(a) (*(volatile uint32_t *)(uintptr_t)(a))
static os_board_io io;
static os_uart uart;
static os_controller controller;
static volatile os_adc_words dma_words;
static volatile os_adc_words sample;
static volatile uint32_t adc_samples,adc_errors,ticks;
static volatile bool adc_valid,board_ready;
static os_bench bench;
static uint32_t lock(void){uint32_t m;__asm volatile("mrs %0, primask\ncpsid i":"=r"(m)::"memory");return m;}
static void unlock(uint32_t m){__asm volatile("msr primask, %0"::"r"(m):"memory");}
static void count(volatile uint32_t *p){if(*p!=UINT32_MAX)++*p;}
static void irq_enable(unsigned n,unsigned priority){
    uint32_t shift=(n&3u)*8u,a=0xe000e400u+(n&~3u);
    REG(0xe000e280u)=1u<<n;REG(a)=(REG(a)&~(255u<<shift))|((priority&3u)<<(shift+6));REG(0xe000e100u)=1u<<n;
}
/* Factory trim lookup and PLL configuration, not a measured frequency. */
static uint32_t clock_hz(void){
    static const uint32_t hz[]={24000000u,22120000u,16000000u,8000000u};
    uint32_t sys=REG(0x40002000u),pll=REG(0x4000203cu),trim=REG(0x4000200cu)&2047u;
    if(((sys>>5)&7u)!=4u||(pll&3u)!=3u)return 0;
    for(unsigned i=0;i<4;i++)if(trim==*(volatile uint16_t *)(uintptr_t)(0x00100c00u+2u*i)){
        uint32_t f=hz[i]*((pll>>5)&15u);return f>>((sys>>8)&7u);
    }
    return 0;
}
void current_irq(void){
    /* Read completed DMA words before clearing the completion flags. */
    if(((REG(0x40021014u)>>16)&7u)==5u){
        __asm volatile("dmb":::"memory");
        uint32_t s=dma_words.scale,a=dma_words.slot2,b=dma_words.slot1;
        if(s<=4095u&&a<=4095u&&b<=4095u){sample=(os_adc_words){s,b,a};adc_valid=true;count(&adc_samples);}
        else{adc_valid=false;count(&adc_errors);}
    }else{adc_valid=false;count(&adc_errors);}
    (void)os_feedback_dma_complete(&io);
}
void systick_irq(void){ticks++;}
void uart_irq(void){os_uart_irq(&uart,&io,ticks);}
void os_target_panic(void){
    if(board_ready){os_hc32_inhibit();os_hc32_indicator(true);}
    for(;;)__asm volatile("wfi");
}
void os_target_main(void){
    io=os_hc32_board_io();os_init(&controller);
    os_offsets offsets;os_board_result status=os_board_prepare(&io,&offsets);
    bench.board_status=(uint8_t)status;
    if(status==OS_BOARD_CLOCK)os_target_panic();
    board_ready=true;os_hc32_inhibit();
    uint32_t hz=clock_hz();bench.core_hz=hz;
    if(hz<1000000u||hz>48000000u)os_target_panic();
    if(!os_uart_init(&uart,&io,hz>>((REG(0x40002000u)>>11)&3u),115200u))os_target_panic();
    os_encoder_bus encoder=os_hc32_encoder_bus();
    if(status==OS_BOARD_OK){
        bench.offset1=offsets.slot1;bench.offset2=offsets.slot2;
        os_board_step_init(&io,false);REG(0x40003050u)|=1u; /* counter only */
        irq_enable(4,0);os_feedback_dma_start(&io,(uint32_t)(uintptr_t)&dma_words);
    }
    irq_enable(6,2);REG(0xe000ed20u)=(REG(0xe000ed20u)&0x00ffffffu)|0xc0000000u;
    REG(0xe000e014u)=hz/1000u-1u;REG(0xe000e018u)=0;REG(0xe000e010u)=7u;
    __asm volatile("cpsie i":::"memory");
    uint32_t last=0,last_adc=0;uint8_t in[OS_FRAME_MAX],out[OS_FRAME_MAX];
    for(;;){
        uint32_t m=lock(),now=ticks;
        if(now!=last){
            uint32_t elapsed=now-last;last=now;
            if(elapsed>1u){uint32_t lost=elapsed-1u;bench.missed_ticks=UINT32_MAX-bench.missed_ticks<lost?UINT32_MAX:bench.missed_ticks+lost;}
            bench.ms=now;bench.adc_samples=adc_samples;bench.adc_errors=adc_errors;bench.uart_dropped=uart.dropped;
            bench.scale=(uint16_t)sample.scale;bench.slot1=(uint16_t)sample.slot1;bench.slot2=(uint16_t)sample.slot2;
            bench.flags=(adc_valid&&adc_samples!=last_adc)?8u:0u;last_adc=adc_samples;
            unlock(m);
            if(status==OS_BOARD_OK){
                uint16_t raw=0,angle=0;os_encoder_result er=os_mt6816_read(&encoder,&raw);
                bench.encoder_status=(uint8_t)er;
                if(er==OS_ENC_OK){bench.raw_angle=raw;bench.flags|=2u;
                    if(os_hc32_calibrated_angle(raw,&angle)){bench.cal_angle=angle;bench.flags|=1u;}
                }else if(bench.encoder_errors!=UINT32_MAX)bench.encoder_errors++;
                bench.pulse=(uint16_t)REG(0x40003000u);
                if(REG(0x40020d44u)&128u)bench.flags|=4u;
            }
            /* Status is deliberately OFF; diagnostics carry actual acquisition validity. */
            os_hc32_indicator(status!=OS_BOARD_OK || (bench.flags&11u)!=11u);
        }else unlock(m);
        m=lock();size_t n=os_uart_take(&uart,in);unlock(m);
        if(n){size_t len=os_bench_dispatch(&controller,&bench,in,n,out);
            if(len){m=lock();(void)os_uart_send(&uart,&io,out,len);unlock(m);}}
        /* The regular SysTick also wakes an interrupt arriving before WFI. */
        __asm volatile("wfi");
    }
}
