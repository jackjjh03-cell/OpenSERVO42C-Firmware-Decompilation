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
        os_drive_config_valid(d);
}
static void encode(uint8_t *buf,const os_settings *s,uint32_t gen){
    uint8_t *p=buf;w32(&p,MAGIC);w16(&p,1);w16(&p,OS_SETTINGS_BYTES);w32(&p,gen);
    os_wire_copy(p,&s->protection,OS_CONFIG_WIRE_BYTES);p+=OS_CONFIG_WIRE_BYTES;
    os_wire_copy(p,&s->pid,OS_PID_WIRE_BYTES);p+=OS_PID_WIRE_BYTES;
    os_wire_copy(p,&s->motion,OS_MOTION_WIRE_BYTES);p+=OS_MOTION_WIRE_BYTES;
    w32(&p,(uint32_t)s->limits.minimum);w32(&p,(uint32_t)s->limits.maximum);*p++=(uint8_t)s->limits.enabled;
    const os_drive_config *d=&s->drive;w16(&p,d->microsteps);w16(&p,d->max_encoder_delta);w16(&p,d->phase_limit);w16(&p,d->current_gain_q11);w16(&p,d->compare_limit);*p++=d->geometry;*p++=(uint8_t)d->reverse;w16(&p,d->trip_limit);*p++=s->enable_high;
    w16(&p,os_crc16(buf,90));w16(&p,COMMIT);
}
static bool decode(const uint8_t *buf,os_settings *s,uint32_t *gen){
    const uint8_t *p=buf+90;uint16_t crc=r16(&p),commit=r16(&p);
    if(commit!=COMMIT||crc!=os_crc16(buf,90))return false;
    p=buf;if(r32(&p)!=MAGIC||r16(&p)!=1u||r16(&p)!=OS_SETTINGS_BYTES)return false;*gen=r32(&p);
    os_wire_copy(&s->protection,p,OS_CONFIG_WIRE_BYTES);p+=OS_CONFIG_WIRE_BYTES;
    os_wire_copy(&s->pid,p,OS_PID_WIRE_BYTES);p+=OS_PID_WIRE_BYTES;
    os_wire_copy(&s->motion,p,OS_MOTION_WIRE_BYTES);p+=OS_MOTION_WIRE_BYTES;
    s->limits.minimum=(int32_t)r32(&p);s->limits.maximum=(int32_t)r32(&p);uint8_t enabled=*p++;if(enabled>1u)return false;s->limits.enabled=enabled!=0u;
    os_drive_config *d=&s->drive;d->microsteps=r16(&p);d->max_encoder_delta=r16(&p);d->phase_limit=r16(&p);d->current_gain_q11=r16(&p);d->compare_limit=r16(&p);d->geometry=*p++;uint8_t reverse=*p++;if(reverse>1u)return false;d->reverse=reverse!=0u;d->trip_limit=r16(&p);s->enable_high=*p++;
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
bool os_settings_save(const os_settings_io *io,const os_settings *s){
    if(!os_settings_valid(s))return false;
    os_settings old;uint32_t g=0;unsigned slot=1;uint8_t buf[OS_SETTINGS_BYTES],check[OS_SETTINGS_BYTES];
    bool read_ok;bool found=load(io,&old,&g,&slot,&read_ok);
    if(!read_ok)return false; /* Never erase when either page cannot be inspected. */
    if(found){
        encode(buf,s,g);if(io->read(io->ctx,slot,check,sizeof check)){
            unsigned i=0;while(i<sizeof buf&&buf[i]==check[i])i++;if(i==sizeof buf)return true;
        }
    }
    slot^=1u;encode(buf,s,g+1u);if(!io->erase(io->ctx,slot))return false;
    for(unsigned i=0;i<92;i+=2){const uint8_t *p=buf+i;if(!io->program(io->ctx,slot,i,r16(&p)))return false;}
    if(!io->read(io->ctx,slot,check,sizeof check))return false;
    for(unsigned i=0;i<92;i++)if(buf[i]!=check[i])return false;
    if(check[92]!=255u||check[93]!=255u)return false;
    if(!io->program(io->ctx,slot,92,COMMIT)||!io->read(io->ctx,slot,check,sizeof check))return false;
    for(unsigned i=0;i<sizeof buf;i++)if(buf[i]!=check[i])return false;
    return true;
}
