#include "settings_session.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
typedef struct {uint8_t pages[2][512];uint32_t regs[6];unsigned erases;} mock;
static const uint32_t addresses[]={0x40000c30u,0x40000d30u,0x40000c3cu,0x40000c40u,0x40000d3cu,0x40000d40u};
static unsigned idx(uint32_t a){for(unsigned i=0;i<6;i++)if(a==addresses[i])return i;assert(0);return 0;}
static uint32_t rr(void *p,uint32_t a){return ((mock *)p)->regs[idx(a)];}
static void wr(void *p,uint32_t a,uint32_t v){((mock *)p)->regs[idx(a)]=v;}
static bool rd(void *p,unsigned s,uint8_t *o,unsigned n){memcpy(o,((mock *)p)->pages[s],n);return true;}
static bool er(void *p,unsigned s){mock *m=p;memset(m->pages[s],255,512);m->erases++;return true;}
static bool pg(void *p,unsigned s,unsigned off,uint16_t v){mock *m=p;m->pages[s][off]&=(uint8_t)v;m->pages[s][off+1]&=(uint8_t)(v>>8);return true;}
int main(void){mock m={0};memset(m.pages,255,sizeof m.pages);os_board_io b={&m,rr,wr,0,0};os_settings_io io={&m,rd,er,pg};os_controller c;os_init(&c);os_settings f={c.cfg,c.pid_cfg,c.motion_cfg,c.limits,{16,1000,500,2048,300,25,false,600},0};os_offsets o={2048,2048};os_run r;os_settings_session s;
    assert(os_settings_boot(&s,&r,&b,&io,&f,&o,true));assert(!s.loaded&&!r.drive.control.hal_validated&&!r.gate.enabled);
    r.drive.control.limits=(os_limits){-400,600,true};r.reference_confirmed=true;
    assert(!os_settings_commit(&s,&r,&io,false)&&m.erases==0);r.enable=true;assert(!os_settings_commit(&s,&r,&io,true)&&m.erases==0);r.enable=false;
    assert(os_settings_commit(&s,&r,&io,true)&&s.loaded&&s.generation==1&&m.erases==1);assert(!r.reference_confirmed&&r.drive.control.position_lost);
    assert(os_settings_boot(&s,&r,&b,&io,&f,&o,false));assert(s.loaded&&!r.drive.control.hal_validated&&r.drive.control.limits.maximum==600);
    assert(os_settings_boot(&s,&r,&b,&io,&f,&o,true));assert(r.drive.control.hal_validated&&!r.reference_confirmed&&!r.gate.enabled&&!os_run_arm(&r,&b));
    r.teaching=true;assert(!os_settings_commit(&s,&r,&io,true));r.teaching=false;assert(os_settings_commit(&s,&r,&io,true)&&m.erases==1);
    uint8_t req[8]={0xa5,0x5a,1,9,OS_GET_STORAGE,0,0,0},reply[56];uint16_t crc=os_crc16(req+2,4);req[6]=(uint8_t)crc;req[7]=(uint8_t)(crc>>8);
    assert(os_storage_dispatch(&s,&r,&io,false,req,8,reply)==15&&reply[6]==OS_OK&&reply[7]==1&&reply[9]==1);
    req[4]=OS_SAVE_SETTINGS;crc=os_crc16(req+2,4);req[6]=(uint8_t)crc;req[7]=(uint8_t)(crc>>8);
    assert(os_storage_dispatch(&s,&r,&io,false,req,8,reply)==9&&reply[6]==OS_REJECTED);
    assert(os_storage_dispatch(&s,&r,&io,true,req,8,reply)==9&&reply[6]==OS_OK&&m.erases==1);
    puts("settings session passed");return 0;}
