#include "factory_layout.h"
/* numeric OLED fields. Original caller uses bounded values and
 * zero-filled row buffers. Preserve the original suffix and decimal rules. */
extern int32_t factory_signed_divide(int32_t, int32_t);
void format_telemetry_field(int32_t whole, int32_t fraction, unsigned format, unsigned field, char *out) {
    char reversed[12];
    unsigned n=0, i=(*out=='-');
    do {
        int32_t q=factory_signed_divide(whole, 10);
        reversed[n++]=(char)('0'+whole-q*10);
        whole=q;
    }while (whole>0);
    while (n)out[i++]=reversed[--n];
    if (format!=4) {
        out[i++]='.';
        int32_t q=factory_signed_divide(fraction, 10);
        if (format==3)out[i++]=(char)('0'+q);
        out[i++]=(char)('0'+fraction-q*10);
    }
    if (format==1)out[i++]='K';
    if (field==0)out[i]='?';
    else if (field==1) {
        out[i++]='e';
        out[i++]='r';
        out[i]='r';
    }
    else if (field==2) {
        out[i++]='c';
        if (*out=='-'||i<7)out[i++]='l';
        out[i]='k';
    }
}
extern uint64_t factory_multiply64(uint64_t, uint64_t);
extern void display_three_lines(uint32_t, uint32_t, uint32_t);
static uint64_t read_pair(uint32_t a) {
    uint32_t l=RAM32(a), h=RAM32(a+4);
    return (uint64_t)l|((uint64_t)h<<32);
}
/* Divide by a small positive constant, preserving quotient sign and avoiding
 * compiler-specific 64-bit division ABI. No dynamic-width shift. */
static int64_t div_small(int64_t value, uint32_t divisor) {
    uint64_t u=(uint64_t)value, q=0;
    uint32_t r=0;
    int neg=value<0;
    if (neg)u=0-u;
    for (unsigned i=0;i<64;i++) {
        r=(r<<1)|(uint32_t)(u>>63);
        u<<=1;
        q<<=1;
        if (r>=divisor) {
            r-=divisor;
            q|=1;
        }
    }
    return (int64_t)(neg?0-q:q);
}
static void copy_literal(char *out, uint32_t a) {
    uint8_t c;
    do {
        c=RAM8(a++);
        *out++=(char)c;
    }while (c);
}
/* Main-screen RPM from calibrated 16-bit-per-turn encoder position.
 * Use foreground SysTick sampling only; no new ISR or motor-loop work. */
extern volatile uint8_t magnet_lock;
extern uint32_t factory_divide(uint32_t,uint32_t);
#define RPM_CTRL RAM32(0xe000e010u)
#define RPM_VAL RAM32(0xe000e018u)
static uint32_t rpm_last,rpm_pos,rpm_ticks,rpm_value;
static uint8_t rpm_owned,rpm_valid,show_pulse;
void rpm_toggle(void){show_pulse^=1u;}
static volatile uint8_t rpm_rebase_pending;
void rpm_rebase(void){rpm_rebase_pending=1;}
void rpm_reset(void){
    if(rpm_owned)RPM_CTRL=0;
    rpm_owned=rpm_valid=0;
}
static void rpm_line(char *out){
    copy_literal(out,TEXT_ADDR("RPM --"));
    if(!LIVE->control_mode || !RAM8(WORK_RAM(WS_DRIVE_ENABLE_STATE)) || !RAM8(WORK_RAM(WS_ENCODER_LUT_READY))){rpm_reset();return;}
    uint32_t pos=RAM32(WORK_RAM(WS_FEEDBACK_POSITION_LOW));
    if(rpm_rebase_pending || !rpm_owned || !(RPM_CTRL&1u)){
        rpm_rebase_pending=0;
        RAM32(0xe000e014u)=0xffffffu;RPM_VAL=0;RPM_CTRL=5;
        rpm_owned=1;rpm_valid=0;rpm_ticks=0;rpm_pos=pos;rpm_last=RPM_VAL;return;
    }
    uint32_t now=RPM_VAL;
    rpm_ticks+=(rpm_last-now)&0xffffffu;rpm_last=now;
    uint32_t clock=RAM32(WORK_RAM(0));
    if(clock && rpm_ticks>=clock/2u){
        uint32_t ms=factory_divide(rpm_ticks,factory_divide(clock,1000u));
        uint32_t d=pos-rpm_pos;if(d>>31)d=0u-d;
        /* A discontinuity is not a valid speed sample. 65536 counts/rev. */
        rpm_valid=(d<=0x1000000u && ms>=450u && ms<=1000u);
        if(rpm_valid)rpm_value=factory_divide((d>>6)*3750u,64u*ms);
        rpm_ticks=0;rpm_pos=pos;
    }
    if(rpm_valid){out[4]=0;out[5]=0;format_telemetry_field((int32_t)rpm_value,0,4,3,out+4);}
}
/* preserve original 32/64-bit wrap and decimal formatting.
 * Names/units remain inferred. The original divides intermediate values
 * using signed 64-bit runtime helpers; output is compared as display bytes. */
void render_telemetry(void) {
    if(magnet_lock){display_three_lines(TEXT_ADDR("Motor lock"),TEXT_ADDR("RPM --"),TEXT_ADDR("Menu"));return;}
    char row[48]={0};
    char *a=row, *b=row+16, *c=row+32;
    int64_t d=(int64_t)(read_pair(WORK_RAM(0x06a0u))-(uint64_t)(int64_t)(int32_t)RAM32(WORK_RAM(0x0668u)));
    if (d<0) {
        *a='-';
        d=(int64_t)(0u-(uint64_t)d);
    }
    d=(int64_t)factory_multiply64((uint64_t)d, 225u)>>12;
    unsigned fmt=0;
    if (d>9999) {
        fmt=1;
        d=div_small(d, 1000);
        if (d>9999) {
            copy_literal(a, *a=='-'?ASSET_ADDR(telemetry_messages, 0x9u):ASSET_ADDR(telemetry_messages, 0x0u));
            fmt=255;
        }
    }
    if (fmt<2) {
        int32_t q=(int32_t)div_small(d, 10);
        format_telemetry_field(q, (int32_t)d-q*10, fmt, 0, a);
    }
    uint32_t err=RAM32(WORK_RAM(WS_POSITION_ERROR));
    if ((int32_t)err<0)err=0u-err;
    d=(int32_t)(err*1125u)>>11;
    fmt=3;
    if (d>99999) {
        fmt=1;
        d=div_small(d, 1000);
        if (d>9999) {
            copy_literal(b, ASSET_ADDR(telemetry_messages, 0x12u));
            fmt=255;
        }
    }
    else if (d>9999)fmt=2;
    if (fmt<4) {
        int32_t q=(int32_t)div_small(d, 100);
        format_telemetry_field(q, (int32_t)d-q*100, fmt, 1, b);
    }
    if(!show_pulse)rpm_line(c);
    else {
        rpm_reset();
        d=(int64_t)(read_pair(WORK_RAM(WS_MOTION_POSITION))-(uint64_t)(int64_t)(int32_t)RAM32(WORK_RAM(0x0664u)));
        if (d<0) {
            *c='-';
            d=(int64_t)(0u-(uint64_t)d);
        }
        fmt=4;
        int32_t frac=0;
        if (d>999) {
            fmt=1;
            int64_t q=div_small(d, 100);
            frac=(int32_t)((uint32_t)q-(uint32_t)div_small(q, 10)*10u);
            d=div_small(d, 1000);
            if (d>99) {
                copy_literal(c, *c=='-'?ASSET_ADDR(telemetry_messages, 0x24u):ASSET_ADDR(telemetry_messages, 0x1bu));
                fmt=255;
            }
        }
        if (fmt<5)format_telemetry_field((int32_t)d, frac, fmt, 2, c);
    }
    a[10]=b[10]=c[10]=0;
    display_three_lines((uint32_t)(uintptr_t)a, (uint32_t)(uintptr_t)b, (uint32_t)(uintptr_t)c);
}
