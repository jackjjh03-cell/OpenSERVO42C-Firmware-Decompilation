#include "oled.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define CTRL 0x40000408u
#define DATA 0x4000040cu
#define STATUS 0x40000414u
typedef struct {uint32_t ctrl,status;uint8_t bytes[256];unsigned n,starts,stops,delays,reads;bool stuck,nack;} fake;
static uint32_t read(void *ctx,uint32_t a){fake *f=ctx;f->reads++;if(a==CTRL)return f->ctrl;if(a==STATUS)return f->status;return 0;}
static void write(void *ctx,uint32_t a,uint32_t v){fake *f=ctx;
    /* Display init must not touch any motor timer or ADC register. */
    assert(a<0x40000c00u||a>=0x40000e00u);assert(a<0x40002400u||a>=0x40002500u);
    if(a==DATA){assert(f->n<sizeof f->bytes);f->bytes[f->n++]=(uint8_t)v;return;}
    if(a!=CTRL)return;
    uint32_t old=f->ctrl;f->ctrl=v;
    if(v&16u){f->stops++;f->ctrl&=~16u;return;}
    if(f->stuck){f->ctrl&=~8u;return;}
    if((v&32u)&&!(old&32u)){f->starts++;f->status=8u;f->ctrl|=8u;return;}
    if((old&8u)&&!(v&8u)){f->status=f->status==8u?(f->nack?32u:24u):40u;f->ctrl|=8u;}
}
static void delay(void *ctx,uint32_t us){fake *f=ctx;assert(us==100000u);f->delays++;}
int main(void){fake f={0};os_board_io io={&f,read,write,0,delay};
    assert(!os_oled_init(&io,1000000u)&&!f.n);
    assert(os_oled_init(&io,48000000u));assert(f.delays==2&&f.starts==1&&f.stops==1&&f.n==31);
    assert(f.bytes[0]==120&&f.bytes[1]==0&&f.bytes[2]==0xae&&f.bytes[30]==0xaf);
    uint8_t pixels[72]={0};pixels[0]=42;pixels[71]=99;
    unsigned n=f.n;assert(!os_oled_page(&io,5,pixels)&&f.n==n);
    assert(os_oled_page(&io,4,pixels));assert(f.n==n+79&&f.starts==3&&f.stops==3);
    const uint8_t head[]={120,0,0xb4,12,17,120,0x40};assert(!memcmp(f.bytes+n,head,sizeof head));
    assert(f.bytes[n+7]==42&&f.bytes[f.n-1]==99);
    f.nack=true;n=f.n;assert(!os_oled_page(&io,0,pixels)&&f.n==n+1);
    f.nack=false;f.stuck=true;n=f.reads;assert(!os_oled_page(&io,0,pixels)&&f.reads-n<6100u);
    os_oled_view v={.position=INT32_MIN,.adc_errors=UINT32_MAX,.mode=5,.state=2,.flags=2};
    os_oled_render(&v,false,2,pixels);for(unsigned i=0;i<5;i++)assert(pixels[i]==8);assert(pixels[5]==0);
    for(unsigned i=66;i<72;i++)assert(pixels[i]==0);
    os_oled_render(&v,true,2,pixels);assert(pixels[66]!=0||pixels[67]!=0);
    os_oled_render(&v,false,9,pixels);for(unsigned i=0;i<72;i++)assert(!pixels[i]);
    os_oled_buttons b={0};os_oled_button_sample(&b,4);assert(!b.diagnostics);
    os_oled_button_sample(&b,0);os_oled_button_sample(&b,4);assert(!b.diagnostics);
    os_oled_button_sample(&b,4);assert(b.diagnostics);os_oled_button_sample(&b,4);assert(b.diagnostics);
    os_oled_button_sample(&b,0);os_oled_button_sample(&b,0);os_oled_button_sample(&b,1);os_oled_button_sample(&b,1);assert(!b.diagnostics);
    os_oled_button_sample(&b,3);os_oled_button_sample(&b,0);assert(!b.diagnostics);
    puts("OLED tests passed");return 0;
}
