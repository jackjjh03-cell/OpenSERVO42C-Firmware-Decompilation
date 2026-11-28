#include "settings.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
typedef struct {uint8_t page[2][OS_SETTINGS_PAGE];int budget;unsigned erases;bool read_fail;} fake;
static bool read(void *p,unsigned s,uint8_t *out,unsigned n){fake *f=p;assert(s<2&&n<=OS_SETTINGS_PAGE);if(f->read_fail&&s==0)return false;memcpy(out,f->page[s],n);return true;}
static bool erase(void *p,unsigned s){fake *f=p;assert(s<2);if(f->budget--<=0)return false;memset(f->page[s],255,OS_SETTINGS_PAGE);f->erases++;return true;}
static bool program(void *p,unsigned s,unsigned off,uint16_t v){fake *f=p;assert(s<2&&off%2==0&&off+1<OS_SETTINGS_PAGE);if(f->budget--<=0)return false;f->page[s][off]&=(uint8_t)v;f->page[s][off+1]&=(uint8_t)(v>>8);return true;}
int main(void){fake f;memset(&f,255,sizeof f);f.budget=100;f.erases=0;f.read_fail=false;os_settings_io io={&f,read,erase,program};os_controller c;os_init(&c);
    os_settings s={c.cfg,c.pid_cfg,c.motion_cfg,{-100,100,true},{16,1000,500,2048,300,25,false,600},0},out;
    uint32_t gen;unsigned slot;assert(!os_settings_load(&io,&out,&gen,&slot));assert(os_settings_save(&io,&s));assert(os_settings_load(&io,&out,&gen,&slot)&&gen==1&&out.limits.minimum==-100);
    /* Independently packed version-1 record: retain byte-for-byte compatibility. */
    static const uint8_t golden[94]={
        0x4f,0x53,0x56,0x31,0x01,0x00,0x5e,0x00,0x01,0x00,0x00,0x00,0x00,0x10,0x00,0x00,
        0x00,0x08,0x00,0x00,0x64,0x00,0x00,0x00,0x20,0x03,0x64,0x00,0x64,0x00,0x00,0x00,
        0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x40,
        0x1f,0x10,0x27,0x00,0x00,0x30,0x75,0x00,0x00,0x98,0x3a,0x00,0x00,0x60,0xea,0x00,
        0x00,0x01,0x9c,0xff,0xff,0xff,0x64,0x00,0x00,0x00,0x01,0x10,0x00,0xe8,0x03,0xf4,
        0x01,0x00,0x08,0x2c,0x01,0x19,0x00,0x58,0x02,0x00,0xce,0x88,0x5a,0xa5};
    assert(!memcmp(f.page[slot],golden,sizeof golden));
    assert(os_settings_save(&io,&s)&&f.erases==1);fake original=f;s.limits.maximum=200;
    f.read_fail=true;assert(!os_settings_save(&io,&s)&&f.erases==1);f=original;
    for(int cut=0;cut<48;cut++){f=original;f.budget=cut;assert(!os_settings_save(&io,&s));assert(os_settings_load(&io,&out,&gen,&slot)&&gen==1&&out.limits.maximum==100);}
    f=original;f.budget=100;assert(os_settings_save(&io,&s));assert(os_settings_load(&io,&out,&gen,&slot)&&gen==2&&out.limits.maximum==200);
    f.page[slot][20]^=1;assert(os_settings_load(&io,&out,&gen,&slot)&&gen==1);
    s.drive.trip_limit=s.drive.phase_limit;assert(!os_settings_save(&io,&s));puts("settings tests passed");return 0;
}
