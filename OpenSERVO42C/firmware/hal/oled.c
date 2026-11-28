#include "oled.h"
#define CTRL 0x40000408u
#define DATA 0x4000040cu
#define STATUS 0x40000414u
#define R(a) io->read(io->ctx,(a))
#define W(a,v) io->write(io->ctx,(a),(v))
static bool wait(const os_board_io *io,uint32_t bit,bool set){
    for(unsigned i=0;i<6000u;i++)if(((R(CTRL)&bit)!=0u)==set)return true;
    return false;
}
static bool transmit(const os_board_io *io,uint8_t control,const uint8_t *data,unsigned count){
    if(!wait(io,16u,false))return false;
    W(CTRL,R(CTRL)|32u);
    bool ok=wait(io,8u,true)&&R(STATUS)==8u;
    if(ok){W(CTRL,R(CTRL)&~32u);W(DATA,120u);W(CTRL,R(CTRL)&~8u);ok=wait(io,8u,true)&&R(STATUS)==24u;}
    if(ok){W(DATA,control);W(CTRL,R(CTRL)&~8u);ok=wait(io,8u,true)&&R(STATUS)==40u;}
    for(unsigned i=0;ok&&i<count;i++){
        W(DATA,data[i]);W(CTRL,R(CTRL)&~8u);ok=wait(io,8u,true)&&R(STATUS)==40u;
    }
    W(CTRL,(R(CTRL)&~40u)|16u);return ok;
}
__attribute__((noinline)) bool os_oled_init(const os_board_io *io,uint32_t hz){
    /* Factory display bus and reset sequence; no motor timer changes. */
    if(hz<6400000u||hz>48000000u)return false;
    os_board_oled_pins(io);
    W(0x40002028u,R(0x40002028u)&~16u);W(0x40002028u,R(0x40002028u)|16u);
    W(CTRL,0);W(CTRL,0x40);W(0x40000400u,1);
    uint32_t div=hz/3200000u-1u;
    if(div<9u)W(CTRL,R(CTRL)|1u);
    W(0x40000404u,div);W(CTRL,R(CTRL)|64u);
    W(0x40020d14u,R(0x40020d14u)|4096u);io->delay_us(io->ctx,100000u);
    W(0x40020d10u,R(0x40020d10u)|4096u);io->delay_us(io->ctx,100000u);
    static const uint8_t commands[]={0xae,0xd5,0xf0,0xa8,0x27,0xd3,0,0x40,0x8d,0x14,0x20,2,0xa1,200,0xda,0x12,0xad,0x30,0x81,0x40,0xd9,0x22,0xdb,0x20,0xa4,0xa6,0xc,0x11,0xaf};
    return transmit(io,0,commands,sizeof commands);
}
__attribute__((noinline)) bool os_oled_page(const os_board_io *io,unsigned page,const uint8_t pixels[72]){
    if(page>=5u)return false;
    uint8_t commands[]={(uint8_t)(0xb0u+page),12,17};
    return transmit(io,0,commands,sizeof commands)&&transmit(io,0x40,pixels,72);
}
/* Five column ASCII subset, most-significant bit unused. */
static const uint8_t font[][5]={
    {0x3e,0x51,0x49,0x45,0x3e},{0,0x42,0x7f,0x40,0},{0x42,0x61,0x51,0x49,0x46},
    {0x21,0x41,0x45,0x4b,0x31},{0x18,0x14,0x12,0x7f,0x10},{0x27,0x45,0x45,0x45,0x39},
    {0x3c,0x4a,0x49,0x49,0x30},{1,0x71,9,5,3},{0x36,0x49,0x49,0x49,0x36},{6,0x49,0x49,0x29,0x1e},
    {0x7e,0x11,0x11,0x11,0x7e},{0x7f,0x49,0x49,0x49,0x36},{0x3e,0x41,0x41,0x41,0x22},
    {0x7f,0x41,0x41,0x22,0x1c},{0x7f,0x49,0x49,0x49,0x41},{0x7f,9,9,9,1},
    {0x3e,0x41,0x49,0x49,0x7a},{0x7f,8,8,8,0x7f},{0,0x41,0x7f,0x41,0},
    {0x20,0x40,0x41,0x3f,1},{0x7f,8,0x14,0x22,0x41},{0x7f,0x40,0x40,0x40,0x40},
    {0x7f,2,0x0c,2,0x7f},{0x7f,4,8,0x10,0x7f},{0x3e,0x41,0x41,0x41,0x3e},
    {0x7f,9,9,9,6},{0x3e,0x41,0x51,0x21,0x5e},{0x7f,9,0x19,0x29,0x46},
    {0x46,0x49,0x49,0x49,0x31},{1,1,0x7f,1,1},{0x3f,0x40,0x40,0x40,0x3f},
    {0x1f,0x20,0x40,0x20,0x1f},{0x3f,0x40,0x38,0x40,0x3f},{0x63,0x14,8,0x14,0x63},
    {7,8,0x70,8,7},{0x61,0x51,0x49,0x45,0x43}
};
static unsigned text(uint8_t *p,unsigned x,const char *s){
    for(;*s&&x<12u;s++,x++){
        unsigned i=(unsigned char)*s;const uint8_t *g=0;
        if(i>='0'&&i<='9')g=font[i-'0'];else if(i>='A'&&i<='Z')g=font[10u+i-'A'];
        for(unsigned k=0;k<5u;k++)p[x*6u+k]=g?g[k]:i=='-'?8u:0u;
    }
    return x;
}
static void number(uint8_t *p,const char *label,uint32_t n,bool negative){
    char s[12];unsigned i=11;s[i]=0;
    do{s[--i]=(char)('0'+n%10u);n/=10u;}while(n);
    if(negative)s[--i]='-';
    unsigned x=text(p,0,label);text(p,x,s+i);
}
__attribute__((noinline)) void os_oled_render(const os_oled_view *v,uint8_t screen,unsigned page,uint8_t pixels[72]){
    for(unsigned i=0;i<72u;i++)pixels[i]=0;
    if(screen==OS_OLED_DIAG){
        switch(page){
        case 0:text(pixels,0,"DIAGNOSTICS");break;
        case 1:number(pixels,"RAW ",v->raw_angle,false);break;
        case 2:number(pixels,"A ",v->adc_errors,false);break;
        case 3:number(pixels,"E ",v->encoder_errors,false);break;
        case 4:number(pixels,"BOARD ",v->board_status,false);break;
        }return;
    }
    static const char *const states[]={"OFF","READY","ACTIVE","FAULT"};
    static const char *const modes[]={"CNC","POSITION","VELOCITY","JOG","TORQUE","OPEN LOOP"};
    if(screen==OS_OLED_SETUP){
        switch(page){
        case 0:text(pixels,0,"SETUP");break;
        case 1:text(pixels,0,v->mode<6u?modes[v->mode]:"UNKNOWN");break;
        case 2:text(pixels,0,(v->flags&6u)?"DISABLE EN":"NEXT MODE");break;
        case 3:text(pixels,0,v->faults?"ENTER ACK":"NO FAULT");break;
        case 4:text(pixels,0,"MENU EXIT");break;
        }return;
    }
    switch(page){
    case 0:text(pixels,0,v->board_status?"BOARD ERROR":v->state==2u&&!(v->flags&2u)?"INHIBITED":v->state<4u?states[v->state]:"UNKNOWN");break;
    case 1:text(pixels,0,v->mode<6u?modes[v->mode]:"UNKNOWN");break;
    case 2:number(pixels,"",v->position<0?0u-(uint32_t)v->position:(uint32_t)v->position,v->position<0);break;
    case 3:number(pixels,"LOAD PM ",v->load,false);break;
    case 4:if(v->faults)number(pixels,"FAULT ",v->faults,false);else text(pixels,0,(v->flags&1u)?"REF OK":"NO REF");break;
    }
}
uint8_t os_oled_button_sample(os_oled_buttons *b,uint8_t pressed){
    pressed&=7u;
    uint8_t action=OS_OLED_NONE;
    if(pressed==b->previous&&pressed!=b->stable){
        uint8_t old=b->stable;b->stable=pressed;
        /* Require a full release before each single-key action. */
        if(!old){
            if(pressed==4u)b->screen=b->screen==OS_OLED_SETUP?OS_OLED_STATUS:(uint8_t)(b->screen+1u);
            else if(b->screen==OS_OLED_SETUP){
                if(pressed==1u)action=OS_OLED_MODE;
                else if(pressed==2u)action=OS_OLED_ACK;
            }
        }
    }
    b->previous=pressed;return action;
}
