#include "board.h"
#define R(a) io->read(io->ctx,(a))
#define W(a,v) io->write(io->ctx,(a),(v))
#define F(a,m,v) W((a),(R(a)&~(uint32_t)(m))|((uint32_t)(v)&(uint32_t)(m)))
#define B(a,b,v) F((a),1u<<(b),(v)?1u<<(b):0u)
#define SYS 0x40002000u
#define ADC 0x40002400u
#define GPIO 0x40020d00u
#define TMR 0x40000c00u
static void key(const os_board_io *io,uint32_t a){W(a,0x5a5a);W(a,0xa5a5);}
static bool wait_set(const os_board_io *io,uint32_t a,uint32_t mask){
    for(unsigned n=0;n<100000u;n++)if(R(a)&mask)return true;
    return false;
}
static void pin(const os_board_io *io,uint32_t port,unsigned p,unsigned cfg,uint32_t mux){
    uint32_t base=GPIO+port+12u;
    B(base,p,0);W(0x40020c00u+port+4u*p,0);
    B(base-12u,p,cfg&1u);B(base+16u,p,cfg&2u);B(base+20u,p,cfg&4u);
    B(base+24u,p,cfg&8u);B(base+32u,p,cfg&16u);B(0x40020f08u,15,cfg&32u);
    if(mux)W(0x40020c00u+port+4u*p,mux);
}
/* V1.1.3 board_startup.c, clock_and_bus.c and application_flow.c. */
static bool clock_init(const os_board_io *io){
    F(SYS+0x14,1023u,io->trim(io->ctx,0x00100c22u));
    F(SYS+0xc,2047u,io->trim(io->ctx,0x00100c08u));
    uint32_t saved=R(SYS+0x20);F(SYS+0x20,0x10000000u,0x10000000u);
    static const uint16_t masks[4]={0xfff4,0x08fb,0xc000,0xff0b};
    static const uint16_t bits[4]={0x000b,0xf704,0x3fff,0x00f4};
    for(unsigned i=0;i<4;i++)W(GPIO+64u*i+12u,R(GPIO+64u*i+12u)&masks[i]);
    for(unsigned i=0;i<4;i++)W(GPIO+64u*i,R(GPIO+64u*i)|bits[i]);
    for(unsigned i=0;i<4;i++)W(GPIO+64u*i+32u,R(GPIO+64u*i+32u)|bits[i]);
    W(SYS+0x20,saved);key(io,SYS+8);F(SYS,0x700u,0x700u);
    const uint32_t trims[4]={0x00100c08u,0x00100c06u,0x00100c04u,0x00100c00u};
    for(unsigned i=0;i<4;i++)W(SYS+0xc,io->trim(io->ctx,trims[i]));
    key(io,SYS+8);F(SYS,0x700u,0);
    key(io,0x4002002cu);F(0x40020020u,12u,4u);
    F(SYS+0x3c,0x6000u,0x6000u);F(SYS+0x3c,0x1cu,0x10u);
    F(SYS+0x3c,0x1e0u,0x40u);F(SYS+0x3c,3u,3u);F(SYS+0x3c,0x38000u,0x38000u);
    saved=R(SYS);key(io,SYS+8);B(SYS+0x20,16,1);B(ADC,0,1);io->delay_us(io->ctx,100);
    B(SYS,4,1);
    if(!wait_set(io,SYS+0x3c,0x40000u)){W(SYS,saved);return false;}
    W(SYS,saved);key(io,SYS+8);F(SYS,0xe0u,0x80u);
    static const uint8_t gates[]={30,28,6,4,0,8,10,11,16,29,27};
    for(unsigned i=0;i<sizeof gates;i++)B(SYS+0x20,gates[i],1);
    return true;
}
static void pwm_init(const os_board_io *io){
    /* Keep the factory timer fields but configure with the output gate closed. */
    for(unsigned t=0;t<2;t++){
        uint32_t a=TMR+t*0x100u;
        F(a+0x30,0x1800u,0);B(a+0xc,0,0);
        F(a+0xc,0x0802707eu,0x3008u);F(a,0xffffu,1200u);B(a+0xc,7,1);
        F(a+0x3c,0xffffu,1200);F(a+0x40,0xffffu,1200);
        F(a+0x24,0x30f0u,0xc0u);F(a+0x1c,0xffu,0x77u);F(a+0xc,0x600000u,0);
        F(a+0x34,255u,1);F(a+4,0xffffu,0);F(a+0x18,0x7f7u,t?0x220u:0x11u);
    }
    const uint8_t ports[]={0,64,64,0},pins[]={15,3,6,2},mux[]={5,2,3,4};
    for(unsigned i=0;i<4;i++){B(GPIO+ports[i]+20u,pins[i],1);pin(io,ports[i],pins[i],32,mux[i]);}
}
static void encoder_init(const os_board_io *io){
    for(unsigned p=4;p<=7;p++)if(p!=6){if(p<6)B(GPIO+16u,p,1);pin(io,0,p,32,1);}
    pin(io,0,6,39,1);W(0x40000800u,0);W(0x40000800u,0x5eu);W(0x40000808u,0);W(0x40000804u,1);
}
static void adc_init(const os_board_io *io){
    B(GPIO+64u+12u,0,1);B(GPIO+64u+12u,1,1);B(GPIO+64u+12u,11,1);
    B(ADC+4,0,1);B(ADC,0,1);io->delay_us(io->ctx,1000);
    W(ADC+4,1);io->delay_us(io->ctx,100);W(ADC+4,R(ADC+4)|0x2608u);
    F(ADC+8,0x404u,0x400u);F(ADC+8,0x200u,0);
    F(ADC+0x4c,0x300000u,0x200000u);
    F(ADC+0x4c,0x7fffu,8u|(9u<<5)|(18u<<10));F(ADC+0xb4,0x20u,0);
}
os_board_result os_board_prepare(const os_board_io *io,os_offsets *offsets){
    if(!clock_init(io))return OS_BOARD_CLOCK;
    pwm_init(io);B(GPIO+128u+20u,14,1);pin(io,128,14,32,0);
    pin(io,0,9,39,0);pin(io,0,10,39,0);pin(io,64,4,39,0);
    if(!(R(GPIO+4)&(1u<<9)))return OS_BOARD_BUTTON;
    encoder_init(io);adc_init(io);
    os_offset_cal cal;os_offset_begin(&cal);
    for(unsigned i=0;i<OS_CURRENT_OFFSET_SAMPLES;i++){
        W(ADC+0xc8,1);io->delay_us(io->ctx,5000);
        if(!wait_set(io,ADC+0xb0,0x20u)){W(ADC+0xc8,0);return OS_BOARD_ADC;}
        uint32_t a=R(ADC+0x98),b=R(ADC+0x94);F(ADC+0xb4,0x20u,0);
        if(a>4095u||b>4095u||!os_offset_add(&cal,(uint16_t)a,(uint16_t)b)){W(ADC+0xc8,0);return OS_BOARD_ADC;}
    }
    W(ADC+0xc8,0);return os_offset_finish(&cal,offsets)?OS_BOARD_OK:OS_BOARD_OFFSET;
}
void os_board_enable_irq_init(const os_board_io *io){
    B(0x40020e50u,7,0);B(0x40020d78u,7,1);B(0x40020d7cu,7,1);
}
bool os_board_enable_irq_take(const os_board_io *io){
    if(!(R(0x40020e40u)&128u))return false;
    B(0x40020e50u,7,0);return true;
}
void os_board_step_init(const os_board_io *io,bool reverse){
    pin(io,64,7,7,0);pin(io,0,11,33,7);pin(io,0,8,33,6);
    const uint32_t c=0x40003000u;
    F(c+0x50,0x17fu,0x100u);W(c+4,65535u);
    W(c+0x88,reverse?64u:16u);W(c+0x8c,reverse?16u:64u);
    W(c+0x7c,0);F(c+0x68,0x33u,0x33u);F(c,0xffffu,0);
    /* Counter stays stopped until the control owner arms it. */
}
