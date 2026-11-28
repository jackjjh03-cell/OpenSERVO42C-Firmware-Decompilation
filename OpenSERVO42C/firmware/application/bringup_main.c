/* Shared acquisition and motor-runtime application. Motor runtime is a
 * separate, unvalidated bench build. No identity or LUT writes. */
#include "board.h"
#include "bringup.h"
#include "encoder.h"
#include "phase.h"
#include "feedback_dma.h"
#include "hc32_inhibit.h"
#include "uart_transport.h"
#include "settings_session.h"
#include "settings_flash.h"
#include "run_protocol.h"
#include "timing.h"
#include "calibration_check.h"
#include "trace.h"
#ifndef OS_MOTOR_RUNTIME
#define OS_MOTOR_RUNTIME 0
#endif
#define REG(a) (*(volatile uint32_t *)(uintptr_t)(a))
static os_board_io io;
static os_uart uart;
static os_run run;
static os_settings_session session;
static os_flash flash;
static os_settings_io store;
static volatile os_adc_words dma_words;
static volatile os_adc_words sample;
static volatile uint32_t adc_samples,adc_errors,ticks;
static volatile bool adc_valid,board_ready;
static os_bench bench;
static os_calibration_check calibration;
static size_t calibration_reply(const uint8_t *in,size_t n,uint8_t *out){
    if(!os_frame_valid(in,n))return 0;
    out[6]=OS_OK;
    if(in[5]){out[6]=OS_BAD_LENGTH;return os_frame_reply(in,out,1);}
    os_wire_put32(out+7,(uint32_t)calibration.winding);
    os_wire_put16(out+11,calibration.max_delta);os_wire_put16(out+13,calibration.forward);
    os_wire_put16(out+15,calibration.reverse);os_wire_put16(out+17,calibration.flat);
    out[19]=calibration.flags;return os_frame_reply(in,out,14);
}
static os_encoder_bus encoder;
static bool input_enabled(void){return ((REG(0x40020d44u)&128u)!=0u)==(session.enable_high!=0u);}
static void read_encoder(void){
    uint16_t raw=0,angle=0;os_encoder_result er=os_mt6816_read(&encoder,&raw);
    bench.flags=(uint8_t)(bench.flags&~3u);bench.encoder_status=(uint8_t)er;
    if(er==OS_ENC_OK){bench.raw_angle=raw;bench.flags|=2u;
        if(calibration.flags==(OS_CAL_MARKERS|OS_CAL_WINDING)&&os_hc32_calibrated_angle(raw,&angle)){bench.cal_angle=angle;bench.flags|=1u;}
    }else if(bench.encoder_errors!=UINT32_MAX)bench.encoder_errors++;
}
#if OS_MOTOR_RUNTIME
static uint32_t position_adc;
static os_trace trace;
static os_timing_stat timing[3]; /* current, position, foreground commands */
static uint32_t timing_start(void){(void)REG(0xe000e010u);return REG(0xe000e018u);}
static bool timing_end(unsigned index,uint32_t start){
    bool wrapped=(REG(0xe000e010u)&0x10000u)!=0u;
    uint32_t end=REG(0xe000e018u);
    return os_timing_record(&timing[index],REG(0xe000e014u)+1u,start,end,wrapped);
}
static size_t timing_reply(const uint8_t *in,size_t n,uint8_t *out){
    if(!os_frame_valid(in,n))return 0;
    out[6]=OS_OK;
    if(in[5]){out[6]=OS_BAD_LENGTH;return os_frame_reply(in,out,1);}
    for(unsigned i=0;i<3;i++){
        os_wire_put32(out+7u+8u*i,timing[i].peak);
        os_wire_put32(out+11u+8u*i,timing[i].overruns);
    }
    return os_frame_reply(in,out,25);
}
static void service_enable_edge(void){
    if(os_board_enable_irq_take(&io))os_run_enable_edge(&run,&io,input_enabled());
}
void enable_irq(void){service_enable_edge();}
/* IRQs 4 and SysTick run at the same priority: neither can preempt the other.
 * Foreground dispatch takes PRIMASK only while it accesses the shared owner. */
static void timing_trip(void){
    os_gate_close(&run.gate,&io);run.drive.pending_fault|=OS_F_TIMING;
}
#endif
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
#if OS_MOTOR_RUNTIME
    uint32_t start=timing_start();
#endif
    /* Read completed DMA words before clearing the completion flags. */
    if(((REG(0x40021014u)>>16)&7u)==5u){
        __asm volatile("dmb":::"memory");
        uint32_t s=dma_words.scale,a=dma_words.slot2,b=dma_words.slot1;
        if(s<=4095u&&a<=4095u&&b<=4095u){sample=(os_adc_words){s,b,a};adc_valid=true;count(&adc_samples);}
        else{adc_valid=false;count(&adc_errors);}
    }else{adc_valid=false;count(&adc_errors);}
    (void)os_feedback_dma_complete(&io);
#if OS_MOTOR_RUNTIME
    service_enable_edge();
    read_encoder();
    service_enable_edge();
    os_run_adc(&run,&io,input_enabled(),(bench.flags&1u)!=0u,bench.cal_angle,adc_valid,
               (uint16_t)sample.scale,(uint16_t)sample.slot2,(uint16_t)sample.slot1);
    /* If another conversion has already completed, the current loop has no
     * execution margin. Never continue driving with a known backlog. */
    bool late=timing_end(0,start);
    if(run.gate.enabled&&(late||((REG(0x40021014u)>>16)&7u)!=0u))timing_trip();
#endif
}
void systick_irq(void){
    ticks++;
#if OS_MOTOR_RUNTIME
    if(!run.drive.configured)return;
    uint32_t start=timing_start();
    service_enable_edge();
    bool fresh=run.adc_count!=position_adc;position_adc=run.adc_count;
    bench.ms=ticks;bench.adc_samples=adc_samples;bench.adc_errors=adc_errors;
    bench.scale=(uint16_t)sample.scale;bench.slot1=(uint16_t)sample.slot1;bench.slot2=(uint16_t)sample.slot2;
    bench.pulse=(uint16_t)REG(0x40003000u);bench.uart_dropped=uart.dropped;
    bench.flags=(uint8_t)((bench.flags&3u)|((REG(0x40020d44u)&128u)?4u:0u)|(fresh&&adc_valid?8u:0u));
    os_run_tick(&run,&io,ticks,bench.cal_angle,fresh&&(bench.flags&1u)!=0u,bench.pulse,input_enabled(),false,false);
    bench.missed_ticks=run.missed_ticks;
    os_trace_tick(&trace,&run.drive.control,ticks);
    os_hc32_indicator(run.drive.control.state==OS_FAULT);
    if(timing_end(1,start)&&run.gate.enabled)timing_trip();
#endif
}
void uart_irq(void){os_uart_irq(&uart,&io,ticks);}
void os_target_panic(void){
    __asm volatile("cpsid i":::"memory");
    if(board_ready){os_hc32_inhibit();os_hc32_indicator(true);}
    for(;;)__asm volatile("wfi");
}
/* Runs only with output disabled. Pause IRQs and
 * DMA during flash activity; the session invalidates the mechanical reference. */
static size_t storage_request(const uint8_t *in,size_t n,uint8_t *out){
    if(in[4]!=OS_SAVE_SETTINGS||n!=8u||!os_frame_valid(in,n))
        return os_storage_dispatch(&session,&run,&store,false,in,n,out);
    uint32_t m=lock();
    bool en=input_enabled();
    if(en||run.enable||run.teaching||run.gate.enabled||run.drive.control.state!=OS_OFF||run.drive.pending_fault||uart.tx_len){
        unlock(m);return os_storage_dispatch(&session,&run,&store,false,in,n,out);
    }
    os_gate_close(&run.gate,&io);flash.write_permitted=true;
    if(!os_flash_prepare_timing(&flash,bench.core_hz)){
        flash.write_permitted=false;unlock(m);return os_storage_dispatch(&session,&run,&store,false,in,n,out);
    }
    #if OS_MOTOR_RUNTIME
    trace.running=false;
    #endif
    REG(0xe000e010u)=0;REG(0xe000e180u)=1u<<4;
    REG(0x40000c0cu)&=~1u;os_feedback_dma_stop(&io);
    size_t len=os_storage_dispatch(&session,&run,&store,true,in,n,out);
    flash.write_permitted=false;
    if(!os_flash_idle(&flash))os_target_panic();
    adc_valid=false;REG(0xe000e280u)=1u<<4;
    os_feedback_dma_start(&io,(uint32_t)(uintptr_t)&dma_words);irq_enable(4,0);
    REG(0xe000ed04u)=1u<<25;REG(0xe000e018u)=0;REG(0xe000e010u)=7u;
    unlock(m);return len;
}
void os_target_main(void){
    io=os_hc32_board_io();os_init(&run.drive.control);
    os_offsets offsets;os_board_result status=os_board_prepare(&io,&offsets);
    bench.board_status=(uint8_t)status;
    if(status==OS_BOARD_CLOCK)os_target_panic();
    board_ready=true;os_hc32_inhibit();
    uint32_t hz=clock_hz();bench.core_hz=hz;
    if(hz<1000000u||hz>48000000u)os_target_panic();
    if(!os_uart_init(&uart,&io,hz>>((REG(0x40002000u)>>11)&3u),115200u))os_target_panic();
    encoder=os_hc32_encoder_bus();
    calibration=os_hc32_check_calibration();
    if(status==OS_BOARD_OK){
        bench.offset1=offsets.slot1;bench.offset2=offsets.slot2;
        flash=os_hc32_settings_flash();store=os_flash_settings_io(&flash);
        const os_controller *c=&run.drive.control;
        const os_settings fallback={c->cfg,c->pid_cfg,c->motion_cfg,c->limits,{16,1000,500,2048,300,25,false,600},0};
        if(!os_settings_boot(&session,&run,&io,&store,&fallback,&offsets,OS_MOTOR_RUNTIME!=0))os_target_panic();
        os_board_step_init(&io,false);REG(0x40003050u)|=1u; /* counter only */
        if(OS_MOTOR_RUNTIME){os_board_enable_irq_init(&io);irq_enable(1,0);}
        irq_enable(4,0);os_feedback_dma_start(&io,(uint32_t)(uintptr_t)&dma_words);
    }
    irq_enable(6,2);REG(0xe000ed20u)=(REG(0xe000ed20u)&0x00ffffffu)|(OS_MOTOR_RUNTIME?0u:0xc0000000u);
    REG(0xe000e014u)=hz/1000u-1u;REG(0xe000e018u)=0;REG(0xe000e010u)=7u;
    __asm volatile("cpsie i":::"memory");
#if !OS_MOTOR_RUNTIME
    uint32_t last=0,last_adc=0,last_error=0;
#endif
    uint8_t in[OS_FRAME_MAX],out[OS_FRAME_MAX];
    for(;;){
        uint32_t m;
#if !OS_MOTOR_RUNTIME
        m=lock();uint32_t now=ticks;
        if(now!=last){
            uint32_t elapsed=now-last;last=now;
            if(elapsed>1u){uint32_t lost=elapsed-1u;bench.missed_ticks=UINT32_MAX-bench.missed_ticks<lost?UINT32_MAX:bench.missed_ticks+lost;}
            bench.ms=now;bench.adc_samples=adc_samples;bench.adc_errors=adc_errors;bench.uart_dropped=uart.dropped;
            bench.scale=(uint16_t)sample.scale;bench.slot1=(uint16_t)sample.slot1;bench.slot2=(uint16_t)sample.slot2;
            bench.flags=(adc_valid&&adc_samples!=last_adc&&adc_errors==last_error)?8u:0u;
            bool adc_failed=adc_errors!=last_error;last_adc=adc_samples;last_error=adc_errors;
            unlock(m);
            if(status==OS_BOARD_OK){
                read_encoder();
                bench.pulse=(uint16_t)REG(0x40003000u);
                if(REG(0x40020d44u)&128u)bench.flags|=4u;
                bool en=((bench.flags&4u)!=0u)==(session.enable_high!=0u);
                if(adc_failed||(bench.flags&8u))os_run_adc(&run,&io,en,(bench.flags&1u)!=0u,bench.cal_angle,!adc_failed,bench.scale,bench.slot2,bench.slot1);
                os_run_tick(&run,&io,now,bench.cal_angle,(bench.flags&1u)!=0u,bench.pulse,en,false,false);
            }
            /* Motor authorization remains false in this acquisition application. */
            os_hc32_indicator(status!=OS_BOARD_OK || (bench.flags&11u)!=11u);
        }else unlock(m);
#endif
        m=lock();size_t n=os_uart_take(&uart,in);unlock(m);
        if(n){size_t len;
#if OS_MOTOR_RUNTIME
            m=lock();
            uint32_t start=timing_start();
            if(status==OS_BOARD_OK){service_enable_edge();os_run_enable(&run,&io,input_enabled());}
#endif
#if OS_MOTOR_RUNTIME
            if(in[4]>=OS_TRACE_START&&in[4]<=OS_TRACE_READ)len=os_trace_dispatch(&trace,in,n,out);
            else if(in[4]==30u)len=timing_reply(in,n,out);
            else
#endif
            if(in[4]==35u)len=calibration_reply(in,n,out);
            else if(status==OS_BOARD_OK && in[4]>=OS_GET_STORAGE && in[4]<=OS_SET_DRIVE)len=storage_request(in,n,out);
            else if(status==OS_BOARD_OK&&in[4]!=OS_GET_BENCH)len=os_run_dispatch(&run,&io,in,n,out);
            else len=os_bench_dispatch(&run.drive.control,&bench,in,n,out);
#if OS_MOTOR_RUNTIME
            if(status==OS_BOARD_OK){service_enable_edge();os_run_enable(&run,&io,input_enabled());}
            if(len)(void)os_uart_send(&uart,&io,out,len);
            /* Save pauses SysTick, so do not mix flash maintenance with command timing. */
            if(in[4]!=OS_SAVE_SETTINGS&&timing_end(2,start)&&run.gate.enabled)timing_trip();
            unlock(m);
#else
            if(len){m=lock();(void)os_uart_send(&uart,&io,out,len);unlock(m);}
#endif
        }
        /* The regular SysTick also wakes an interrupt arriving before WFI. */
        __asm volatile("wfi");
    }
}
