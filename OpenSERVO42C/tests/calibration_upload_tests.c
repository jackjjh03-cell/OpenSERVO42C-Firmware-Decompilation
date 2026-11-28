#include "calibration_upload.h"
#include "phase.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
/* Model the full address space, including factory metadata that must not be written. */
typedef struct {uint16_t words[32768];uint32_t ctrl,gates;unsigned lut_erases,lut_programs;int budget;} mock;
static bool writable(uint32_t a){return (a>=0x7600u&&a<0x7a00u)||(a>=0x8000u&&a<0x10000u);}
static uint32_t rd(void *p,uint32_t a){mock *m=p;if(a==0x40020020u)return m->ctrl;if(a==0x40000c30u||a==0x40000d30u)return m->gates;assert(0);return 0;}
static void wr(void *p,uint32_t a,uint32_t v){mock *m=p;if(a==0x40020020u)m->ctrl=v;else assert(a==0x4002002cu||a==0x40020030u);}
static uint8_t rb(void *p,uint32_t a){mock *m=p;assert(a<65536u);return (uint8_t)(m->words[a/2u]>>((a&1u)*8u));}
static void er(void *p,uint32_t a){mock *m=p;assert(writable(a)&&!(a&511u)&&(m->ctrl&3u)==2);if(m->budget--<=0)return;if(a>=0x8000u)m->lut_erases++;for(unsigned i=0;i<256;i++)m->words[a/2u+i]=65535u;}
static void pg(void *p,uint32_t a,uint16_t v){mock *m=p;assert(writable(a)&&!(a&1u)&&(m->ctrl&3u)==1);if(m->budget--<=0)return;if(a>=0x8000u)m->lut_programs++;m->words[a/2u]&=v;}
static uint8_t cmd(os_lut_upload *u,os_settings_session *s,os_flash *f,os_settings_io *store,os_calibration_check *c,const uint16_t *lut,bool q,uint8_t op,const uint8_t *data,uint8_t size){
    uint8_t in[OS_FRAME_MAX]={0xa5,0x5a,1,0,op,size},out[OS_FRAME_MAX];if(size)memcpy(in+6,data,size);os_wire_put16(in+6+size,os_crc16(in+2,4u+size));
    assert(os_lut_dispatch(u,s,f,store,c,lut,q,in,8u+size,out)>=9u&&os_frame_valid(out,8u+out[5]));return out[6];
}
static uint16_t desired[OS_LUT_ENTRIES];
static void chunk(uint8_t data[34],uint16_t off){os_wire_put16(data,off);for(unsigned i=0;i<16;i++)os_wire_put16(data+2+2*i,desired[off+i]);}
int main(void){
    static mock m;memset(m.words,255,sizeof m.words);m.budget=100000;
    for(unsigned i=0x7a00u/2u;i<0x8000u/2u;i++)m.words[i]=(uint16_t)i;
    os_flash f={&m,rd,wr,rb,er,pg,true};os_settings_io store=os_flash_settings_io(&f);
    os_controller c;os_init(&c);os_settings settings={c.cfg,c.pid_cfg,c.motion_cfg,c.limits,{16,1000,500,2048,300,25,false,600},0,0,0},out;
    assert(os_settings_save(&store,&settings));os_settings_session s={.loaded=true};os_lut_upload u={0};os_calibration_check check={.flags=3};const uint16_t *lut=m.words+0x4000u;
    for(unsigned i=0;i<OS_LUT_ENTRIES;i++)desired[i]=(uint16_t)(i*4u+12345u);
    uint8_t crc[4],data[34];os_wire_put32(crc,os_calibration_crc(desired));
    assert(cmd(&u,&s,&f,&store,&check,lut,false,OS_LUT_BEGIN,crc,4)==OS_REJECTED&&check.flags==3);
    m.budget=51;assert(cmd(&u,&s,&f,&store,&check,lut,true,OS_LUT_BEGIN,crc,4)==OS_REJECTED&&!u.active&&!check.flags&&!m.lut_erases);
    chunk(data,0);assert(cmd(&u,&s,&f,&store,&check,lut,true,OS_LUT_WRITE,data,34)==OS_REJECTED&&!m.lut_erases);
    m.budget=100000;assert(cmd(&u,&s,&f,&store,&check,lut,true,OS_LUT_BEGIN,crc,4)==OS_OK&&u.active);
    uint32_t gen;unsigned slot;assert(os_settings_load(&store,&out,&gen,&slot)&&out.calibration_state==OS_CAL_DIRTY);
    chunk(data,16);assert(cmd(&u,&s,&f,&store,&check,lut,true,OS_LUT_WRITE,data,34)==OS_REJECTED&&!m.lut_erases);
    chunk(data,0);m.gates=0x1000u;assert(cmd(&u,&s,&f,&store,&check,lut,true,OS_LUT_WRITE,data,34)==OS_REJECTED&&!u.active&&!m.lut_erases);m.gates=0;
    assert(cmd(&u,&s,&f,&store,&check,lut,true,OS_LUT_BEGIN,crc,4)==OS_OK);
    m.budget=2;assert(cmd(&u,&s,&f,&store,&check,lut,true,OS_LUT_WRITE,data,34)==OS_REJECTED&&!u.active);
    assert(os_settings_load(&store,&out,&gen,&slot)&&out.calibration_state==OS_CAL_DIRTY);
    m.budget=100000;assert(cmd(&u,&s,&f,&store,&check,lut,true,OS_LUT_BEGIN,crc,4)==OS_OK);
    assert(cmd(&u,&s,&f,&store,&check,lut,true,OS_LUT_FINISH,0,0)==OS_REJECTED);
    unsigned erases=m.lut_erases;
    for(unsigned off=0;off<OS_LUT_ENTRIES;off+=16){chunk(data,(uint16_t)off);assert(cmd(&u,&s,&f,&store,&check,lut,true,OS_LUT_WRITE,data,34)==OS_OK);}
    assert(m.lut_erases-erases==64u&&u.next==OS_LUT_ENTRIES);
    assert(!memcmp(lut,desired,sizeof desired));
    assert(cmd(&u,&s,&f,&store,&check,lut,true,OS_LUT_FINISH,0,0)==OS_OK&&!u.active&&!check.flags);
    assert(os_settings_load(&store,&out,&gen,&slot)&&out.calibration_state==OS_CAL_VERIFIED&&out.calibration_crc==os_calibration_crc(desired));
    assert(os_check_managed_calibration(lut,out.calibration_state,out.calibration_crc,0,0).flags==3);
    /* Failed publication and later corruption remain unusable. */
    m.words[0x4001u]^=1;assert(!os_check_managed_calibration(lut,out.calibration_state,out.calibration_crc,1,1).flags);
    assert(cmd(&u,&s,&f,&store,&check,lut,true,OS_LUT_BEGIN,crc,4)==OS_OK);
    assert(cmd(&u,&s,&f,&store,&check,lut,false,OS_LUT_ABORT,0,0)==OS_OK&&!u.active);
    assert(os_settings_load(&store,&out,&gen,&slot)&&out.calibration_state==OS_CAL_DIRTY);
    for(unsigned i=0x7a00u/2u;i<0x8000u/2u;i++)assert(m.words[i]==(uint16_t)i);
    assert(!os_flash_lut_write(&f,16384u,data)&&!os_flash_lut_write(&f,1u,data));
    puts("calibration upload tests passed");return 0;
}
