#include "settings.h"
#include "protocol.h"
#define MAGIC 0x3156534fu
#define COMMIT 0xa55au
static void w16(uint8_t **p,uint16_t x){os_wire_put16(*p,x);*p+=2;}
static void w32(uint8_t **p,uint32_t x){os_wire_put32(*p,x);*p+=4;}
static uint16_t r16(const uint8_t **p){uint16_t x=os_wire_get16(*p);*p+=2;return x;}
static uint32_t r32(const uint8_t **p){uint32_t x=os_wire_get32(*p);*p+=4;return x;}
bool os_settings_valid(const os_settings *s){
    const os_drive_config *d=&s->drive;
    return os_config_valid(&s->protection)&&os_pid_config_valid(&s->pid)&&os_motion_config_valid(&s->motion)&&
        s->limits.minimum<s->limits.maximum&&s->enable_high<=1u&&
        os_drive_config_valid(d)&&s->calibration_state<=OS_CAL_VERIFIED&&
        (s->calibration_state==OS_CAL_VERIFIED||s->calibration_crc==0u);
}
static unsigned encode(uint8_t *buf,const os_settings *s,uint32_t gen){
    unsigned len=s->calibration_state==OS_CAL_LEGACY?OS_SETTINGS_LEGACY_BYTES:OS_SETTINGS_BYTES;
    uint8_t *p=buf;w32(&p,MAGIC);w16(&p,(uint16_t)(len==OS_SETTINGS_LEGACY_BYTES?1u:2u));w16(&p,(uint16_t)len);w32(&p,gen);
    os_wire_copy(p,&s->protection,OS_CONFIG_WIRE_BYTES);p+=OS_CONFIG_WIRE_BYTES;
    os_wire_copy(p,&s->pid,OS_PID_WIRE_BYTES);p+=OS_PID_WIRE_BYTES;
    os_wire_copy(p,&s->motion,OS_MOTION_WIRE_BYTES);p+=OS_MOTION_WIRE_BYTES;
    w32(&p,(uint32_t)s->limits.minimum);w32(&p,(uint32_t)s->limits.maximum);*p++=(uint8_t)s->limits.enabled;
    const os_drive_config *d=&s->drive;w16(&p,d->microsteps);w16(&p,d->max_encoder_delta);w16(&p,d->phase_limit);w16(&p,d->current_gain_q11);w16(&p,d->compare_limit);*p++=d->geometry;*p++=(uint8_t)d->reverse;w16(&p,d->trip_limit);*p++=s->enable_high;
    if(len==OS_SETTINGS_BYTES){w32(&p,s->calibration_crc);*p++=s->calibration_state;*p++=0;}
    w16(&p,os_crc16(buf,len-4u));w16(&p,COMMIT);return len;
}
static bool decode(const uint8_t *buf,os_settings *s,uint32_t *gen){
    const uint8_t *p=buf;if(r32(&p)!=MAGIC)return false;
    uint16_t version=r16(&p),len=r16(&p);
    if(!((version==1u&&len==OS_SETTINGS_LEGACY_BYTES)||(version==2u&&len==OS_SETTINGS_BYTES)))return false;
    *gen=r32(&p);const uint8_t *tail=buf+len-4u;uint16_t crc=r16(&tail),commit=r16(&tail);
    if(commit!=COMMIT||crc!=os_crc16(buf,len-4u))return false;
    os_wire_copy(&s->protection,p,OS_CONFIG_WIRE_BYTES);p+=OS_CONFIG_WIRE_BYTES;
    os_wire_copy(&s->pid,p,OS_PID_WIRE_BYTES);p+=OS_PID_WIRE_BYTES;
    os_wire_copy(&s->motion,p,OS_MOTION_WIRE_BYTES);p+=OS_MOTION_WIRE_BYTES;
    s->limits.minimum=(int32_t)r32(&p);s->limits.maximum=(int32_t)r32(&p);uint8_t enabled=*p++;if(enabled>1u)return false;s->limits.enabled=enabled!=0u;
    os_drive_config *d=&s->drive;d->microsteps=r16(&p);d->max_encoder_delta=r16(&p);d->phase_limit=r16(&p);d->current_gain_q11=r16(&p);d->compare_limit=r16(&p);d->geometry=*p++;uint8_t reverse=*p++;if(reverse>1u)return false;d->reverse=reverse!=0u;d->trip_limit=r16(&p);s->enable_high=*p++;
    s->calibration_state=OS_CAL_LEGACY;s->calibration_crc=0;
    if(version==2u){s->calibration_crc=r32(&p);s->calibration_state=*p++;if(*p||s->calibration_state==OS_CAL_LEGACY)return false;}
    return os_settings_valid(s);
}
static bool load(const os_settings_io *io,os_settings *s,uint32_t *gen,unsigned *slot,bool *read_ok){
    *read_ok=true;
    uint8_t buf[OS_SETTINGS_BYTES];os_settings candidate;uint32_t g;bool found=false;
    for(unsigned i=0;i<2;i++){
        if(!io->read(io->ctx,i,buf,sizeof buf)){*read_ok=false;continue;}
        if(!decode(buf,&candidate,&g))continue;
        if(!found||(g!=*gen&&(uint32_t)(g-*gen)<0x80000000u)){*s=candidate;*gen=g;*slot=i;found=true;}
    }
    return found;
}
bool os_settings_load(const os_settings_io *io,os_settings *s,uint32_t *gen,unsigned *slot){
    bool read_ok;return load(io,s,gen,slot,&read_ok);
}
static bool save(const os_settings_io *io,const os_settings *s,bool force){
    if(!os_settings_valid(s))return false;
    os_settings old;uint32_t g=0;unsigned slot=1;uint8_t buf[OS_SETTINGS_BYTES],check[OS_SETTINGS_BYTES];
    bool read_ok;bool found=load(io,&old,&g,&slot,&read_ok);
    if(!read_ok)return false;
    /* Never silently downgrade managed calibration to factory markers. */
    if(found&&old.calibration_state!=OS_CAL_LEGACY&&s->calibration_state==OS_CAL_LEGACY)return false;
    unsigned len=encode(buf,s,g);
    if(found&&!force&&io->read(io->ctx,slot,check,len)){
        unsigned i=0;while(i<len&&buf[i]==check[i])i++;if(i==len)return true;
    }
    slot^=1u;len=encode(buf,s,g+1u);if(!io->erase(io->ctx,slot))return false;
    for(unsigned i=0;i<len-2u;i+=2){const uint8_t *p=buf+i;if(!io->program(io->ctx,slot,i,r16(&p)))return false;}
    if(!io->read(io->ctx,slot,check,len))return false;
    for(unsigned i=0;i<len-2u;i++)if(buf[i]!=check[i])return false;
    if(check[len-2u]!=255u||check[len-1u]!=255u)return false;
    if(!io->program(io->ctx,slot,len-2u,COMMIT)||!io->read(io->ctx,slot,check,len))return false;
    for(unsigned i=0;i<len;i++)if(buf[i]!=check[i])return false;
    return true;
}
bool os_settings_save(const os_settings_io *io,const os_settings *s){return save(io,s,false);}
bool os_settings_calibration_pending(const os_settings_io *io,os_settings *s){
    s->calibration_state=OS_CAL_DIRTY;s->calibration_crc=0;
    /* A failed first or second transaction must prevent LUT writes. Once both
     * are committed, even falling back to the older slot cannot authorize a LUT. */
    return save(io,s,true)&&save(io,s,true);
}
