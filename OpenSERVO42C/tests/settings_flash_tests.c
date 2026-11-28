#include "settings_flash.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
typedef struct {uint8_t bytes[1024];uint32_t ctrl,gates,timing[8];unsigned writes,erases,programs;bool fail;} mock;
static unsigned offset(uint32_t a){assert(a>=OS_SETTINGS_SLOT0&&a<OS_SETTINGS_SLOT1+OS_SETTINGS_PAGE);return a-OS_SETTINGS_SLOT0;}
static uint32_t read_reg(void *ctx,uint32_t a){mock *m=ctx;if(a>=0x40020000u&&a<0x40020020u)return m->timing[(a-0x40020000u)/4u];if(a==0x40020020u)return m->ctrl;if(a==0x40000c30u||a==0x40000d30u)return m->gates;assert(0);return 0;}
static void write_reg(void *ctx,uint32_t a,uint32_t v){mock *m=ctx;m->writes++;if(a>=0x40020000u&&a<0x40020020u){if(!m->fail)m->timing[(a-0x40020000u)/4u]=v;}else if(a==0x40020020u)m->ctrl=v;else assert(a==0x4002002cu||a==0x40020030u);}
static uint8_t read_byte(void *ctx,uint32_t a){return ((mock *)ctx)->bytes[offset(a)];}
static void erase_trigger(void *ctx,uint32_t a){mock *m=ctx;assert((m->ctrl&3)==2&&offset(a)%512==0);m->erases++;if(!m->fail)memset(m->bytes+offset(a),255,512);}
static void program_halfword(void *ctx,uint32_t a,uint16_t v){mock *m=ctx;assert((m->ctrl&3)==1&&!(a&1));m->programs++;if(!m->fail){m->bytes[offset(a)]&=(uint8_t)v;m->bytes[offset(a+1)]&=(uint8_t)(v>>8);}}
int main(void){mock m={0};os_flash f={&m,read_reg,write_reg,read_byte,erase_trigger,program_halfword,false};os_settings_io io=os_flash_settings_io(&f);uint8_t buf[512];
    assert(!io.erase(io.ctx,0)&&!io.program(io.ctx,0,0,123)&&m.writes==0);
    f.write_permitted=true;m.gates=0x1000;assert(!io.erase(io.ctx,0)&&!io.program(io.ctx,0,0,123)&&m.writes==0);m.gates=0;
    assert(!io.erase(io.ctx,2)&&!io.program(io.ctx,2,0,123)&&!io.program(io.ctx,0,511,123)&&!io.program(io.ctx,0,1,123)&&!io.read(io.ctx,0,buf,513));assert(!m.writes);
    assert(!os_flash_prepare_timing(&f,24000000u)&&m.writes==0);
    m.fail=true;assert(!os_flash_prepare_timing(&f,48000000u));m.fail=false;
    assert(os_flash_prepare_timing(&f,48000000u)&&m.timing[0]==384&&m.timing[1]==255&&m.timing[4]==1680000&&m.timing[7]==12000&&os_flash_idle(&f));
    assert(io.erase(io.ctx,0)&&m.erases==1&&m.ctrl==0);assert(io.program(io.ctx,0,0,0x1234)&&m.ctrl==0);assert(m.programs==1);
    assert(io.read(io.ctx,0,buf,2)&&buf[0]==0x34&&buf[1]==0x12);assert(!io.program(io.ctx,0,0,0xffff)&&m.programs==1);
    m.fail=true;assert(!io.program(io.ctx,0,2,0x1234));assert(!io.erase(io.ctx,0));m.fail=false;
    os_controller c;os_init(&c);os_settings s={c.cfg,c.pid_cfg,c.motion_cfg,{-100,100,true},{16,1000,500,2048,300,25,false,600},0},out;uint32_t gen;unsigned slot;
    assert(os_settings_save(&io,&s));assert(os_settings_load(&io,&out,&gen,&slot)&&gen==1&&out.limits.minimum==-100);
    m.ctrl=16;assert(!io.erase(io.ctx,0)&&!io.read(io.ctx,0,buf,2));puts("flash adapter tests passed");return 0;
}
