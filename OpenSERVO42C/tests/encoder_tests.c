#include "encoder.h"
#include <assert.h>
#include <stdio.h>
typedef struct {uint16_t word;unsigned transfers,fail_at,selects;bool selected;} mock;
static void select(void *ctx,bool on){mock *m=ctx;assert(m->selected!=on);m->selected=on;m->selects++;}
static bool exchange(void *ctx,uint8_t tx,uint8_t *rx){
    mock *m=ctx;static const uint8_t expected[]={0x83,0x55,0x84,0xa0};
    assert(m->selected && m->transfers<4 && tx==expected[m->transfers]);
    unsigned i=m->transfers++;
    if(m->fail_at==i+1)return false;
    *rx=i==1?(uint8_t)(m->word>>8):i==3?(uint8_t)m->word:0;return true;
}
static uint16_t valid_word(uint16_t angle){uint16_t w=(uint16_t)(angle<<2);unsigned ones=0;for(unsigned i=0;i<16;i++)ones+=((uint32_t)w>>i)&1u;return (uint16_t)(w|(ones&1u));}
int main(void){
    uint16_t angle=999;
    for(unsigned i=0;i<16384;i++){
        uint16_t word=valid_word((uint16_t)i);
        assert(os_mt6816_decode(word,&angle)==OS_ENC_OK && angle==i);
        assert(os_mt6816_decode((uint16_t)(word^1u),&angle)==OS_ENC_PARITY);
    }
    assert(os_mt6816_decode(3,&angle)==OS_ENC_WEAK_MAGNET);
    mock m={0};os_encoder_bus bus={&m,exchange,select};
    assert(os_mt6816_read(&bus,&angle)==OS_ENC_OK && angle==0 && !m.selected && m.selects==4);
    for(unsigned i=1;i<=4;i++){
        m=(mock){.fail_at=i};angle=999;
        assert(os_mt6816_read(&bus,&angle)==OS_ENC_TIMEOUT && angle==999 && !m.selected);
    }
    os_encoder_tracker t;os_encoder_reset(&t);
    assert(os_encoder_track(&t,65530,100));assert(os_encoder_track(&t,4,100));assert(t.position==10);
    assert(os_encoder_track(&t,65530,100));assert(t.position==0);
    assert(!os_encoder_track(&t,30000,100));assert(t.previous==65530 && t.position==0);
    puts("Encoder tests passed");return 0;
}
