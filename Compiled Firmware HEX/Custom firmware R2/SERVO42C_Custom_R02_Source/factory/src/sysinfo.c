#include "firmware_api.h"
#include "sysinfo.h"
extern uint32_t gpio_read_input_bit(uint32_t, uint32_t);
extern uint32_t factory_divide(uint32_t, uint32_t);
extern unsigned char __app_flash_end, __stack_budget;
#define BUTTON WORK_RAM(0x5dc)
#define TICK_CTRL RAM32(0xe000e010u)
#define TICK_LOAD RAM32(0xe000e014u)
#define TICK_VAL RAM32(0xe000e018u)
#define MASK 0xffffffu
static uint8_t active, category, page, dirty, owned;
static uint32_t last, elapsed;
#ifdef SYSINFO_PROFILE
/* Diagnostic build only: count the motion handler, not total CPU load.
 * Stop sampling before factory delays borrow SysTick. */
static volatile uint8_t profiling;
static volatile uint32_t irq_count, irq_cycles, irq_peak;
extern void motion_timer_irq(void);
void sysinfo_profile_suspend(void) {profiling=0;}
void sysinfo_profile_irq(void) {
    if(!profiling){motion_timer_irq();return;}
    uint32_t start=TICK_VAL;
    motion_timer_irq();
    if(profiling){uint32_t d=(start-TICK_VAL)&MASK;irq_cycles+=d;if(d>irq_peak)irq_peak=d;irq_count++;}
}
#define EXIT_CATEGORY 10u
#else
#define EXIT_CATEGORY 9u
#endif
/* Use only aligned, explicitly identified RAM fields. Field types 0/1/2 are
 * byte, halfword and word. Type 3 is an eight-digit hexadecimal word. */
typedef struct { char label[4]; uint16_t code; } Field;
#define B(n,o) {n,o}
#define H(n,o) {n,0x4000u|(o)}
#define W(n,o) {n,0x8000u|(o)}
#define X(n,o) {n,0xc000u|(o)}
#define R(f) (WS_RUNTIME_SETTINGS+offsetof(RuntimeSettings,f))
#define M(f) (WS_MOTOR_SETTINGS+offsetof(MotorSettings,f))
#define O(f) (WS_ORIGIN_SETTINGS+offsetof(OriginSettings,f))
#define G(f) (WS_PID_GAINS+offsetof(Gains,f))
static const Field fields[]={
 B("Mode",R(control_mode)),W("Bd",R(uart_baud)),H("Addr",R(uart_address)),
 H("Step",R(microsteps)),H("Ma",M(current_raw)),B("Dir",R(direction)),
 B("En",R(enable_mode)),B("Prot",R(protection)),B("Mply",R(interpolation)),
 B("SDD",R(autosdd)),H("Alt",R(alternate_microsteps)),H("Geom",M(motor_geometry)),
 H("ACC",WS_EXTRA_SETTINGS),H("Trq",R(torque_raw)),H("Lim",R(error_limit)),
 B("0Mod",O(mode)),H("0Spd",O(speed)),B("0Dir",O(direction)),
 H("0Pos",O(position)),H("Save",R(motion_saved_raw)),H("ACix",R(acceleration_index)),
 H("Kp",G(kp)),H("Ki",G(ki)),H("Kd",G(kd)),
 B("Drv",WS_DRIVE_ENABLE_STATE),B("Run",WS_UART_MOTION_STATE),B("Stal",WS_STALL_FLAG),
 X("E",WS_POSITION_ERROR),X("P",WS_MOTION_POSITION),X("T",WS_MOTION_POSITION+8),
 H("Spd",WS_UART_MOTION_SPEED),B("Req",WS_UART_ENABLE_REQUEST),X("U",WS_CONTROL_OUTPUT),
 H("Ang",WS_ENCODER_ANGLE),B("Stat",WS_ENCODER_STATUS),B("LUT",WS_ENCODER_LUT_READY),
 H("Knot",WS_CALIBRATION_KNOT_VALID),H("LCal",WS_CALIBRATION_LUT_VALID),X("F",WS_FEEDBACK_POSITION_LOW),
 B("Boot",WS_MOTION_FEEDBACK),B("Stal",WS_STALL_FLAG),B("Prot",R(protection)),
 X("A",WS_ADC_FEEDBACK_1_RAW),X("B",WS_ADC_FEEDBACK_2_RAW),H("Phas",WS_PHASE_INDEX),
};
/* Custom UART 0x8D shares this read-only field inventory. */
uint32_t sysinfo_read(uint32_t index){
    if(index>=sizeof(fields)/sizeof(fields[0]))return 0xffffffffu;
    unsigned code=fields[index].code,type=code>>14;
    uint32_t a=WORK_RAM(code&0x3fffu);
    return type==0?RAM8(a):type==1?RAM16(a):RAM32(a);
}
/* Keep each page three rows. No live field is sampled by an extra ISR. */
static const char names[][9]={"Credits","Hardware","Settings","PID","Motion","Encoder","Memory","Regs","Faults",
#ifdef SYSINFO_PROFILE
"Perf",
#endif
"Exit"};
static const uint8_t first[]={0,0,0,21,24,33,0,0,39,0};
static const uint8_t pages[]={2,1,7,1,3,2,
#ifdef SYSINFO_PROFILE
1,
#else
2,
#endif
1,2,
#ifdef SYSINFO_PROFILE
1,
#endif
0};
static void decimal(char *s,uint32_t v) {
    char tmp[10]; unsigned n=0;
    do {uint32_t q=factory_divide(v,10);tmp[n++]=(char)('0'+v-q*10);v=q;}while(v);
    while(n)*s++=tmp[--n];*s=0;
}
static void hex8(char *s,uint32_t v) {
    for(unsigned i=0;i<8;i++){unsigned x=v>>28;*s++=(char)(x<10?'0'+x:'A'+x-10);v<<=4;}*s=0;
}
static void text(char *s,const char *t) {while((*s++=*t++)){} }
static void number(char *s,const char *label,uint32_t v,unsigned hex) {
    for(unsigned i=0;i<4&&label[i];i++)*s++=label[i];
    *s++=' ';
    if(hex)hex8(s,v);else decimal(s,v);
}
static void show(char rows[3][11]) {display_three_lines(TEXT_ADDR(rows[0]),TEXT_ADDR(rows[1]),TEXT_ADDR(rows[2]));}
static __attribute__((noinline)) void draw(void) {
    char rows[3][11]={{0}};
    if(page==255){
        for(unsigned i=0;i<3;i++){unsigned c=category+i;if(c>EXIT_CATEGORY)break;rows[i][0]=i?' ':'*';text(rows[i]+1,names[c]);}
    }else if(category==0){
        if(!page){text(rows[0],"SERVO42C-");text(rows[1],"Firmware-");text(rows[2],"Decompilat");}
        else{text(rows[0],"ion");
#ifdef SYSINFO_PROFILE
            text(rows[1],"ProfileR02");
#else
            text(rows[1],"Custom R02");
#endif
            text(rows[2],"V1.1.3");}
    }else if(category==1){
        text(rows[0],"HC32L130");text(rows[1],"Cortex M0+");number(rows[2],"MHz",factory_divide(RAM32(WORK_RAM(0)),1000000u),0);
    }else if(category==6){
#ifdef SYSINFO_PROFILE
        text(rows[0],"Flash 64K");text(rows[1],"RAM 8K");
#else
        if(!page){text(rows[0],"Flash 64K");number(rows[1],"App",(uint32_t)(uintptr_t)&__app_flash_end,0);number(rows[2],"Free",0x7a00u-(uint32_t)(uintptr_t)&__app_flash_end,0);}
        else{text(rows[0],"RAM 8K");number(rows[1],"Stk",(uint32_t)(uintptr_t)&__stack_budget,0);number(rows[2],"Work",WS_TOTAL_BYTES,0);}
#endif
    }else if(category==7){
        number(rows[0],"Part",(RAM32(0xe000ed00u)>>4)&0xfffu,0);number(rows[1],"Div",1u<<((RAM32(0x40002000u)>>8)&7u),0);number(rows[2],"Src",(RAM32(0x40002000u)>>5)&7u,0);
#ifdef SYSINFO_PROFILE
    }else if(category==9){
        uint32_t valid=profiling;profiling=0;
        if(valid&&!dirty&&elapsed>=100){
            uint32_t load=factory_divide(irq_cycles,factory_divide(elapsed,100));
            number(rows[0],"Hz",irq_count*2u,0);
            number(rows[1],"Load",load>100?100:load,0);
            number(rows[2],"C",irq_peak,1);
        }else text(rows[0],"Sampling");
        irq_count=irq_cycles=irq_peak=0;
#endif
    }else{
        unsigned index=first[category]+3u*page;
        for(unsigned i=0;i<3;i++,index++){
            const Field *f=fields+index;unsigned type=f->code>>14;uint32_t v=sysinfo_read(index);
            /* A 32-bit decimal field with a two-character label is bounded to
             * seven digits; corrupt values use full hexadecimal instead. */
            if(type==2&&v>9999999u)number(rows[i],"?",v,1);
            else number(rows[i],f->label,v,type==3);
        }
    }
    show(rows);
}
void sysinfo_open(void){active=1;category=0;page=255;dirty=1;elapsed=0;}
void sysinfo_cancel(void){
#ifdef SYSINFO_PROFILE
    profiling=0;
#endif
    if(owned){TICK_CTRL=0;owned=0;}active=0;}
uint32_t sysinfo_poll(void){
    if(!active)return 0;
    if(RAM32(WORK_RAM(WS_MENU_TABLE_ADDRESS))!=WORK_RAM(0x358u)){sysinfo_cancel();return 0;}
    uint8_t keys=RAM8(BUTTON+1);
    unsigned next=!gpio_read_input_bit(0,9),enter=!gpio_read_input_bit(0,10);
    if(next&&!(keys&1)){dirty=1;RAM16(BUTTON+4)=0;if(page==255){if(++category>EXIT_CATEGORY)category=0;}else if(++page==pages[category])page=0;}
    if(enter&&!(keys&2)){dirty=1;RAM16(BUTTON+4)=0;if(page!=255)page=255;else if(category==EXIT_CATEGORY)sysinfo_cancel();else page=0;}
    RAM8(BUTTON+1)=(uint8_t)((keys&~3u)|next|(enter<<1));
    if(!active)return 1;
#ifdef SYSINFO_PROFILE
    if(category!=9||page==255)profiling=0;
#endif
    /* Factory delays may borrow SysTick and leave it stopped. Rearm on the
     * next foreground iteration; no SysTick interrupt is enabled. */
    if(!(TICK_CTRL&1u)){TICK_LOAD=MASK;TICK_VAL=0;TICK_CTRL=5;owned=1;last=0;elapsed=0;dirty=1;}
    uint32_t now=TICK_VAL;
    if(last)elapsed+=(last-now)&MASK;
    last=now;
    uint32_t interval=RAM32(WORK_RAM(0))/2u;
    if(dirty||(interval&&elapsed>=interval)){
        draw();dirty=0;elapsed=0;last=TICK_VAL;
#ifdef SYSINFO_PROFILE
        if(category==9&&page!=255)profiling=1;
#endif
    }
    return 1;
}
