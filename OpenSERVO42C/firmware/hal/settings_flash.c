#include "settings_flash.h"
#define CTRL 0x40020020u
#define KEY 0x4002002cu
#define FLAGS 0x40020030u
#define R(a) f->read_reg(f->ctx,(a))
#define W(a,v) f->write_reg(f->ctx,(a),(v))
static uint32_t base(unsigned slot){return OS_SETTINGS_SLOT0+slot*OS_SETTINGS_PAGE;}
static void key(os_flash *f){W(KEY,0x5a5au);W(KEY,0xa5a5u);}
static bool idle(os_flash *f){for(unsigned i=0;i<100000u;i++)if(!(R(CTRL)&16u))return true;return false;}
static bool mode(os_flash *f,uint32_t m){
    for(unsigned i=0;i<255u;i++){
        if((R(CTRL)&3u)==m)return true;
        key(f);W(CTRL,(R(CTRL)&~3u)|m);
    }
    return (R(CTRL)&3u)==m;
}
static bool permitted(os_flash *f){return f->write_permitted&&!(R(0x40000c30u)&0x1000u)&&!(R(0x40000d30u)&0x1000u);}
static bool read_page(void *ctx,unsigned slot,uint8_t *out,unsigned n){
    os_flash *f=ctx;if(slot>1u||n>OS_SETTINGS_PAGE||!out||!idle(f))return false;
    for(unsigned i=0;i<n;i++)out[i]=f->read_byte(f->ctx,base(slot)+i);
    return true;
}
static bool begin(os_flash *f,uint32_t m){
    if(!permitted(f)||!idle(f))return false;
    key(f);W(FLAGS,UINT32_MAX);
    return mode(f,m)&&permitted(f);
}
static bool finish(os_flash *f){
    if(!idle(f))return false;
    key(f);W(FLAGS,0);
    return mode(f,0);
}
static bool erase(void *ctx,unsigned slot){
    os_flash *f=ctx;if(slot>1u||!begin(f,2))return false;
    f->erase_trigger(f->ctx,base(slot));if(!finish(f))return false;
    for(unsigned i=0;i<OS_SETTINGS_PAGE;i++)if(f->read_byte(f->ctx,base(slot)+i)!=255u)return false;
    return true;
}
static bool program(void *ctx,unsigned slot,unsigned off,uint16_t v){
    os_flash *f=ctx;if(slot>1u||off>=OS_SETTINGS_PAGE-1u||(off&1u)||!permitted(f)||!idle(f))return false;
    uint32_t a=base(slot)+off;
    uint16_t old=(uint16_t)(f->read_byte(f->ctx,a)|((uint16_t)f->read_byte(f->ctx,a+1u)<<8));
    if((old&v)!=v)return false;
    if(old==v)return true;
    if(!begin(f,1))return false;
    f->program_halfword(f->ctx,a,v);if(!finish(f))return false;
    return f->read_byte(f->ctx,a)==(uint8_t)v&&f->read_byte(f->ctx,a+1u)==(uint8_t)(v>>8);
}
os_settings_io os_flash_settings_io(os_flash *f){return (os_settings_io){f,read_page,erase,program};}
