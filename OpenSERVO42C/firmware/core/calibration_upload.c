#include "calibration_upload.h"
#include "phase.h"
size_t os_lut_dispatch(os_lut_upload *u,os_settings_session *session,os_flash *flash,const os_settings_io *store,
                       os_calibration_check *check,const uint16_t *lut,bool quiesced,const uint8_t *in,size_t n,uint8_t *out){
    if(!os_frame_valid(in,n))return 0;
    uint8_t len=1;out[6]=OS_REJECTED;
    if(in[4]==OS_LUT_STATUS){
        if(in[5])out[6]=OS_BAD_LENGTH;
        else{out[6]=OS_OK;os_wire_put16(out+7,u->next);out[9]=(uint8_t)u->active;out[10]=session->calibration_state;len=5;}
    }else if(in[4]==OS_LUT_ABORT){
        if(in[5])out[6]=OS_BAD_LENGTH;else{u->active=false;out[6]=OS_OK;}
    }else if(quiesced&&session->loaded){
        os_settings s;unsigned slot;uint32_t gen;
        switch(in[4]){
        case OS_LUT_BEGIN:
            if(in[5]!=4u){out[6]=OS_BAD_LENGTH;break;}
            if(u->active||!os_settings_load(store,&s,&gen,&slot))break;
            /* Invalidate RAM before either journal write. Never erase a LUT until
             * both independent records have committed their dirty state. */
            *check=(os_calibration_check){0};session->calibration_state=OS_CAL_DIRTY;session->calibration_crc=0;
            if(!os_settings_calibration_pending(store,&s))break;
            *u=(os_lut_upload){.crc=os_wire_get32(in+6),.active=true};out[6]=OS_OK;break;
        case OS_LUT_WRITE:
            if(in[5]!=34u){out[6]=OS_BAD_LENGTH;break;}
            if(!u->active||u->next!=os_wire_get16(in+6)||u->next>=OS_LUT_ENTRIES)break;
            if(!os_flash_lut_write(flash,u->next,in+8)){u->active=false;break;}
            u->next=(uint16_t)(u->next+16u);out[6]=OS_OK;break;
        case OS_LUT_FINISH:
            if(in[5]){out[6]=OS_BAD_LENGTH;break;}
            if(!u->active||u->next!=OS_LUT_ENTRIES)break;
            u->active=false;
            {os_calibration_check c=os_check_managed_calibration(lut,OS_CAL_VERIFIED,u->crc,0,0);
            if(c.flags!=(OS_CAL_MARKERS|OS_CAL_WINDING)||(c.forward&&c.reverse)||
               !os_settings_load(store,&s,&gen,&slot)||s.calibration_state!=OS_CAL_DIRTY)break;}
            s.calibration_state=OS_CAL_VERIFIED;s.calibration_crc=u->crc;
            if(!os_settings_save(store,&s)||!os_settings_load(store,&s,&gen,&slot)||s.calibration_state!=OS_CAL_VERIFIED||s.calibration_crc!=u->crc)break;
            session->generation=gen;session->calibration_state=OS_CAL_VERIFIED;session->calibration_crc=u->crc;
            /* Keep RAM calibration invalid until reboot rechecks the full LUT. */
            out[6]=OS_OK;break;
        }
    }
    return os_frame_reply(in,out,len);
}
