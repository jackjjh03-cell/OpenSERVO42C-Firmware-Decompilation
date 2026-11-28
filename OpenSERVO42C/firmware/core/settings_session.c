#include "settings_session.h"
bool os_settings_boot(os_settings_session *session,os_run *r,const os_board_io *board,const os_settings_io *store,const os_settings *fallback,const os_offsets *offsets,bool hardware_validated){
    os_settings s;unsigned slot;uint32_t generation=0;
    *session=(os_settings_session){0};
    bool loaded=os_settings_load(store,&s,&generation,&slot);
    if(!loaded)s=*fallback;
    if(!os_settings_valid(&s)||!os_run_init(r,board,&s.drive,offsets))return false;
    os_controller *c=&r->drive.control;
    c->cfg=s.protection;c->pid_cfg=s.pid;c->motion_cfg=s.motion;c->limits=s.limits;
    c->hal_validated=hardware_validated&&loaded;
    session->generation=generation;session->loaded=loaded;session->enable_high=s.enable_high;
    return true;
}
bool os_settings_commit(os_settings_session *session,os_run *r,const os_settings_io *store,bool quiesced){
    os_controller *c=&r->drive.control;
    if(!quiesced||r->gate.enabled||r->enable||r->teaching||c->state!=OS_OFF||c->faults||r->drive.pending_fault)return false;
    os_settings s={c->cfg,c->pid_cfg,c->motion_cfg,c->limits,r->drive.cfg,session->enable_high};
    if(!os_settings_valid(&s))return false;
    /* No encoder observations during flash programming: do not assume shaft stayed still. */
    r->reference_confirmed=false;c->latest.reference_valid=false;c->position_lost=true;
    c->healthy_ms=0;r->adc_good=false;r->tick_seen=false;r->drive.phase_valid=false;
    os_encoder_reset(&r->drive.encoder);
    if(!os_settings_save(store,&s))return false;
    unsigned slot;uint32_t generation;
    if(!os_settings_load(store,&s,&generation,&slot))return false;
    session->generation=generation;session->loaded=true;
    return true;
}
static uint16_t read16(const uint8_t *p){return (uint16_t)(p[0]|((uint16_t)p[1]<<8));}
static void put16(uint8_t *p,uint16_t v){p[0]=(uint8_t)v;p[1]=(uint8_t)(v>>8);}
size_t os_storage_dispatch(os_settings_session *session,os_run *r,const os_settings_io *store,bool quiesced,const uint8_t *in,size_t n,uint8_t *out){
    if(n<8u||in[0]!=0xa5u||in[1]!=0x5au||in[2]!=OS_PROTO_VERSION||in[5]>OS_MAX_PAYLOAD||n!=8u+in[5]||os_crc16(in+2,n-4)!=(uint16_t)(in[n-2]|((uint16_t)in[n-1]<<8)))return 0;
    uint8_t len=1u;out[6]=OS_OK;
    if(in[4]==OS_SET_DRIVE){
        if(in[5]!=15u||in[17]>1u||in[20]>1u)out[6]=OS_BAD_LENGTH;
        else{
            os_drive_config d={read16(in+6),read16(in+8),read16(in+10),read16(in+12),read16(in+14),in[16],in[17]!=0u,read16(in+18)};
            os_controller *c=&r->drive.control;
            os_settings s={c->cfg,c->pid_cfg,c->motion_cfg,c->limits,d,in[20]};
            if(r->gate.enabled||r->enable||r->teaching||c->state!=OS_OFF||r->drive.pending_fault||!os_settings_valid(&s))out[6]=OS_REJECTED;
            else{
                /* Geometry and polarity changes require a new commissioning pass. */
                r->drive.cfg=d;session->enable_high=s.enable_high;
                c->hal_validated=false;c->healthy_ms=0;r->adc_good=false;
                r->reference_confirmed=false;c->latest.reference_valid=false;c->position_lost=true;
                r->drive.remainder=0;r->drive.fast_phase_valid=false;
            }
        }
    }else if(in[5])out[6]=OS_BAD_LENGTH;
    else if(in[4]==OS_GET_DRIVE){
        const os_drive_config *d=&r->drive.cfg;uint8_t *p=out+7;
        put16(p,d->microsteps);put16(p+2,d->max_encoder_delta);put16(p+4,d->phase_limit);put16(p+6,d->current_gain_q11);put16(p+8,d->compare_limit);
        p[10]=d->geometry;p[11]=(uint8_t)d->reverse;put16(p+12,d->trip_limit);p[14]=session->enable_high;len=16;
    }else if(in[4]==OS_GET_STORAGE){
        out[7]=(uint8_t)session->loaded;out[8]=session->enable_high;
        for(unsigned i=0;i<4;i++)out[9+i]=(uint8_t)(session->generation>>(i*8u));
        len=7;
    }else if(in[4]==OS_SAVE_SETTINGS){if(!os_settings_commit(session,r,store,quiesced))out[6]=OS_REJECTED;}
    else out[6]=OS_UNSUPPORTED;
    out[0]=0xa5;out[1]=0x5a;out[2]=OS_PROTO_VERSION;out[3]=in[3];out[4]=(uint8_t)(in[4]|0x80u);out[5]=len;
    uint16_t crc=os_crc16(out+2,4u+len);out[6u+len]=(uint8_t)crc;out[7u+len]=(uint8_t)(crc>>8);return 8u+len;
}
