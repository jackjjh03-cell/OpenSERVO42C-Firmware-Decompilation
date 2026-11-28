#include "encoder.h"
static bool even_parity(uint16_t v){
    v=(uint16_t)(v^(v>>8));v=(uint16_t)(v^(v>>4));
    v=(uint16_t)(v^(v>>2));v=(uint16_t)(v^(v>>1));return (v&1u)==0;
}
os_encoder_result os_mt6816_decode(uint16_t word,uint16_t *angle){
    if(!even_parity(word))return OS_ENC_PARITY;
    if(word&2u)return OS_ENC_WEAK_MAGNET;
    *angle=(uint16_t)(word>>2);return OS_ENC_OK;
}
static bool read_reg(const os_encoder_bus *bus,uint8_t cmd,uint8_t dummy,uint8_t *data){
    uint8_t discard;
    bus->select(bus->context,true);
    bool ok=bus->exchange(bus->context,cmd,&discard) && bus->exchange(bus->context,dummy,data);
    bus->select(bus->context,false);return ok;
}
os_encoder_result os_mt6816_read(const os_encoder_bus *bus,uint16_t *angle){
    uint8_t hi,lo;
    if(!read_reg(bus,0x83,0x55,&hi)||!read_reg(bus,0x84,0xa0,&lo))return OS_ENC_TIMEOUT;
    return os_mt6816_decode((uint16_t)(((uint16_t)hi<<8)|lo),angle);
}
void os_encoder_reset(os_encoder_tracker *t){*t=(os_encoder_tracker){0};}
bool os_encoder_track(os_encoder_tracker *t,uint16_t angle,uint16_t max_step){
    if(!max_step||max_step>=32768u)return false;
    if(!t->initialized){t->previous=angle;t->initialized=true;return true;}
    uint16_t wrapped=(uint16_t)(angle-t->previous);
    int32_t delta=wrapped<32768u?(int32_t)wrapped:(int32_t)wrapped-65536;
    if(delta>(int32_t)max_step||delta<-(int32_t)max_step)return false;
    if((delta>0 && t->position>INT64_MAX-delta)||(delta<0 && t->position<INT64_MIN-delta))return false;
    t->position+=delta;t->previous=angle;return true;
}
