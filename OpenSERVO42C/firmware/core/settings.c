#include "settings.h"
#include "protocol.h"
#define MAGIC 0x3156534fu
#define COMMIT 0xa55au
static void w16(uint8_t **p,uint16_t x){*(*p)++=(uint8_t)x;*(*p)++=(uint8_t)(x>>8);}
static void w32(uint8_t **p,uint32_t x){w16(p,(uint16_t)x);w16(p,(uint16_t)(x>>16));}
static uint16_t r16(const uint8_t **p){uint16_t a=*(*p)++;return (uint16_t)(a|((uint16_t)*(*p)++<<8));}
static uint32_t r32(const uint8_t **p){uint32_t a=r16(p);return a|((uint32_t)r16(p)<<16);}
bool os_settings_valid(const os_settings *s){
    const os_drive_config *d=&s->drive;
    return os_config_valid(&s->protection)&&os_pid_config_valid(&s->pid)&&os_motion_config_valid(&s->motion)&&
        s->limits.minimum<s->limits.maximum&&s->enable_high<=1u&&
        (d->geometry==25u||d->geometry==50u)&&d->microsteps>0u&&d->microsteps<=256u&&
        d->max_encoder_delta>0u&&d->max_encoder_delta<32768u&&d->phase_limit>0u&&d->phase_limit<=2047u&&
        d->trip_limit>d->phase_limit&&d->trip_limit<=4095u&&d->current_gain_q11>0u&&d->compare_limit>0u&&d->compare_limit<=1080u;
}
static void encode(uint8_t *buf,const os_settings *s,uint32_t gen){
    uint8_t *p=buf;w32(&p,MAGIC);w16(&p,1);w16(&p,OS_SETTINGS_BYTES);w32(&p,gen);
    const os_config *c=&s->protection;w32(&p,c->following_counts);w32(&p,c->stall_error_counts);w32(&p,c->stall_speed_counts_s);
    w16(&p,c->stall_load_permille);w16(&p,c->dwell_ms);w16(&p,c->clear_ms);*p++=c->policy;
    const os_pid_config *g=&s->pid;w32(&p,g->kp);w32(&p,g->ki);w32(&p,g->kd);w32(&p,g->deadband);w16(&p,g->output_limit);
    const os_motion_config *m=&s->motion;w32(&p,m->speed);w32(&p,m->accel);w32(&p,m->decel);w32(&p,m->jerk);*p++=m->curve;
    w32(&p,(uint32_t)s->limits.minimum);w32(&p,(uint32_t)s->limits.maximum);*p++=(uint8_t)s->limits.enabled;
    const os_drive_config *d=&s->drive;w16(&p,d->microsteps);w16(&p,d->max_encoder_delta);w16(&p,d->phase_limit);w16(&p,d->current_gain_q11);w16(&p,d->compare_limit);*p++=d->geometry;*p++=(uint8_t)d->reverse;w16(&p,d->trip_limit);*p++=s->enable_high;
    w16(&p,os_crc16(buf,90));w16(&p,COMMIT);
}
static bool decode(const uint8_t *buf,os_settings *s,uint32_t *gen){
    const uint8_t *p=buf+90;uint16_t crc=r16(&p),commit=r16(&p);
    if(commit!=COMMIT||crc!=os_crc16(buf,90))return false;
    p=buf;if(r32(&p)!=MAGIC||r16(&p)!=1u||r16(&p)!=OS_SETTINGS_BYTES)return false;*gen=r32(&p);
    os_config *c=&s->protection;c->following_counts=r32(&p);c->stall_error_counts=r32(&p);c->stall_speed_counts_s=r32(&p);
    c->stall_load_permille=r16(&p);c->dwell_ms=r16(&p);c->clear_ms=r16(&p);c->policy=*p++;
    os_pid_config *g=&s->pid;g->kp=r32(&p);g->ki=r32(&p);g->kd=r32(&p);g->deadband=r32(&p);g->output_limit=r16(&p);
    os_motion_config *m=&s->motion;m->speed=r32(&p);m->accel=r32(&p);m->decel=r32(&p);m->jerk=r32(&p);m->curve=*p++;
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
