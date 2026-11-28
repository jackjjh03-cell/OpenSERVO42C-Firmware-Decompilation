/* Shared acquisition and motor-runtime application. Motor runtime is a
 * separate, unvalidated bench build. Identity and factory metadata are read only. */
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
#include "oled.h"
#include "calibration_sweep.h"
#include "calibration_upload.h"
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
static os_cal_sweep sweep;
static os_lut_upload upload;
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
    if(sweep.flags&OS_CAL_ACTIVE)os_cal_adc(&sweep,&run,&io,input_enabled(),(bench.flags&2u)!=0u,adc_valid,(uint16_t)sample.scale,(uint16_t)sample.slot2,(uint16_t)sample.slot1);
    else os_run_adc(&run,&io,input_enabled(),(bench.flags&1u)!=0u,bench.cal_angle,adc_valid,
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
    if(sweep.flags&OS_CAL_ACTIVE)os_cal_tick(&sweep,&run,&io,ticks,fresh,(bench.flags&2u)!=0u,bench.raw_angle);
    else os_run_tick(&run,&io,ticks,bench.cal_angle,fresh&&(bench.flags&1u)!=0u,bench.pulse,input_enabled(),false,false);
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
    bool lut=false;
#if OS_MOTOR_RUNTIME
    lut=in[4]>=OS_LUT_BEGIN&&in[4]<=OS_LUT_FINISH;
#endif
    if(!os_frame_valid(in,n))return 0;
    if(!lut&&(in[4]!=OS_SAVE_SETTINGS||n!=8u))return os_storage_dispatch(&session,&run,&store,false,in,n,out);
    uint32_t m=lock();
    bool en=input_enabled();
    bool allowed=!en&&!run.enable&&!run.teaching&&!run.gate.enabled&&!run.drive.pending_fault&&!uart.tx_len;
    if(lut)allowed=allowed&&run.drive.control.state!=OS_ACTIVE&&!(run.drive.control.faults&~(uint32_t)(OS_F_ENCODER|OS_F_REFERENCE));
    else allowed=allowed&&os_setup_allowed(&run.drive.control);
    if(!allowed){unlock(m);out[6]=OS_REJECTED;return os_frame_reply(in,out,1);}

    os_gate_close(&run.gate,&io);flash.write_permitted=true;
    if(!os_flash_prepare_timing(&flash,bench.core_hz)){
        flash.write_permitted=false;if(!os_flash_idle(&flash))os_target_panic();unlock(m);out[6]=OS_REJECTED;return os_frame_reply(in,out,1);
    }
    #if OS_MOTOR_RUNTIME
    trace.running=false;
    #endif
    REG(0xe000e010u)=0;REG(0xe000e180u)=1u<<4;
    REG(0x40000c0cu)&=~1u;os_feedback_dma_stop(&io);
    size_t len;
#if OS_MOTOR_RUNTIME
    if(lut){
        run.reference_confirmed=false;run.drive.control.latest.reference_valid=false;run.drive.control.position_lost=true;
        run.drive.control.healthy_ms=0;run.drive.phase_valid=false;run.adc_good=false;run.tick_seen=false;
        os_encoder_reset(&run.drive.encoder);
        len=os_lut_dispatch(&upload,&session,&flash,&store,&calibration,(const uint16_t *)(uintptr_t)0x8000u,true,in,n,out);
    }else
#endif
    len=os_storage_dispatch(&session,&run,&store,true,in,n,out);
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
    bool oled=os_oled_init(&io,hz>>((REG(0x40002000u)>>11)&3u));
    os_oled_buttons buttons={0};os_oled_view view={0};unsigned page=0;uint32_t display_tick=0;
    if(status==OS_BOARD_OK){
        bench.offset1=offsets.slot1;bench.offset2=offsets.slot2;
        flash=os_hc32_settings_flash();store=os_flash_settings_io(&flash);
        const os_controller *c=&run.drive.control;
        const os_settings fallback={c->cfg,c->pid_cfg,c->motion_cfg,c->limits,{16,1000,500,2048,300,25,false,600},0,0,0};
        if(!os_settings_boot(&session,&run,&io,&store,&fallback,&offsets,OS_MOTOR_RUNTIME!=0))os_target_panic();
        calibration=os_hc32_check_calibration(session.calibration_state,session.calibration_crc);
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
            if(in[4]>=OS_CAL_START&&in[4]<=OS_CAL_STOP)len=os_cal_dispatch(&sweep,&run,&io,ticks,(bench.flags&2u)!=0u,bench.raw_angle,in,n,out);
            else if((sweep.flags&OS_CAL_ACTIVE)&&in[4]!=OS_GET_BENCH){
                if(os_frame_valid(in,n)){
                    if(in[4]==OS_DISABLE&&!in[5]){os_cal_stop(&sweep,&run,&io,false);out[6]=OS_OK;}
                    else out[6]=OS_REJECTED;
                    len=os_frame_reply(in,out,1);
                }else len=0;
            }
            else if(in[4]>=OS_LUT_BEGIN&&in[4]<=OS_LUT_ABORT){
                if(in[4]>=OS_LUT_STATUS)len=os_lut_dispatch(&upload,&session,&flash,&store,&calibration,0,false,in,n,out);
                else if(status==OS_BOARD_OK)len=storage_request(in,n,out);
                else if(os_frame_valid(in,n)){out[6]=OS_REJECTED;len=os_frame_reply(in,out,1);}else len=0;
            }
            else if(upload.active&&in[4]!=OS_GET_BENCH){
                if(os_frame_valid(in,n)){
                    if(in[4]==OS_DISABLE&&!in[5]){upload.active=false;os_run_disable(&run,&io);out[6]=OS_OK;}
                    else out[6]=OS_REJECTED;
                    len=os_frame_reply(in,out,1);
                }else len=0;
            }
            else if(in[4]>=OS_TRACE_START&&in[4]<=OS_TRACE_READ)len=os_trace_dispatch(&trace,in,n,out);
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
            if(in[4]!=OS_SAVE_SETTINGS&&!(in[4]>=OS_LUT_BEGIN&&in[4]<=OS_LUT_FINISH)&&timing_end(2,start)&&run.gate.enabled)timing_trip();
            unlock(m);
#else
            if(len){m=lock();(void)os_uart_send(&uart,&io,out,len);unlock(m);}
#endif
        }
        /* Display I/O runs only in the foreground, with IRQs enabled. */
        uint32_t now_display=ticks;
        if(oled&&(uint32_t)(now_display-display_tick)>=40u){
            display_tick=now_display;
            uint8_t pressed=(uint8_t)(((REG(0x40020d04u)&512u)?0u:1u)|((REG(0x40020d04u)&1024u)?0u:2u)|((REG(0x40020d44u)&16u)?0u:4u));
            uint8_t old=buttons.screen,action=os_oled_button_sample(&buttons,pressed);
            if(old!=buttons.screen||action)page=0;
#if OS_MOTOR_RUNTIME
            if(action&&status==OS_BOARD_OK&&!(sweep.flags&OS_CAL_ACTIVE)&&!upload.active){
                m=lock();service_enable_edge();os_run_enable(&run,&io,input_enabled());
                if(action==OS_OLED_MODE)(void)os_run_select_mode(&run,run.drive.control.mode==OS_OPEN_LOOP?OS_CNC:(os_mode)(run.drive.control.mode+1));
                else (void)os_run_ack(&run);
                unlock(m);
            }
#else
            if(buttons.screen==OS_OLED_SETUP)buttons.screen=OS_OLED_STATUS;
#endif
            if(!page){
                m=lock();const os_controller *c=&run.drive.control;
                view=(os_oled_view){c->latest.actual,c->faults,adc_errors,bench.encoder_errors,c->latest.load_permille,bench.raw_angle,
                    (uint8_t)(run.gate.enabled?OS_ACTIVE:c->state),(uint8_t)c->mode,(uint8_t)status,(uint8_t)((run.reference_confirmed?1u:0u)|(run.gate.enabled?2u:0u)|(run.enable?4u:0u))};
                unlock(m);
            }
            uint8_t pixels[72];os_oled_render(&view,buttons.screen,page,pixels);
            oled=os_oled_page(&io,page,pixels);page=(page+1u)%5u;
        }
        /* The regular SysTick also wakes an interrupt arriving before WFI. */
        __asm volatile("wfi");
    }
}
