#include "current.h"
#define ADC_MAX 4095u
static bool valid_offset(uint16_t v){return v>=1861u && v<=2234u;}
void os_offset_begin(os_offset_cal *c){*c=(os_offset_cal){.min={ADC_MAX,ADC_MAX}};}
bool os_offset_add(os_offset_cal *c,uint16_t slot2,uint16_t slot1){
    if(c->count>=OS_CURRENT_OFFSET_SAMPLES || slot2>ADC_MAX || slot1>ADC_MAX){c->invalid=true;return false;}
    uint16_t v[2]={slot2,slot1};
    for(unsigned i=0;i<2;i++){
        c->sum[i]+=v[i];if(v[i]<c->min[i])c->min[i]=v[i];if(v[i]>c->max[i])c->max[i]=v[i];
    }
    c->count++;return true;
}
bool os_offset_finish(const os_offset_cal *c,os_offsets *out){
    if(c->invalid || c->count!=OS_CURRENT_OFFSET_SAMPLES)return false;
    uint16_t a=(uint16_t)((c->sum[0]-c->min[0]-c->max[0]+8u)>>4);
    uint16_t b=(uint16_t)((c->sum[1]-c->min[1]-c->max[1]+8u)>>4);
    if(!valid_offset(a)||!valid_offset(b))return false;
    *out=(os_offsets){a,b};return true;
}
bool os_current_feedback(const os_offsets *o,uint16_t slot2,uint16_t slot1,os_pair *out){
    if(!valid_offset(o->slot2)||!valid_offset(o->slot1)||slot2>ADC_MAX||slot1>ADC_MAX)return false;
    *out=(os_pair){(int32_t)o->slot2-slot2,(int32_t)slot1-o->slot1};return true;
}
static int32_t rounded(int32_t n,unsigned bits){
    uint32_t m=(uint32_t)(n<0?-n:n);
    int32_t v=(int32_t)((m+(1u<<(bits-1u)))>>bits);return n<0?-v:v;
}
static int32_t clamp(int32_t x,int32_t lim){return x>lim?lim:x< -lim?-lim:x;}
void os_pwm_inactive(os_pwm_compare *out){*out=(os_pwm_compare){1200,1200,1200,1200};}
static uint16_t compare_value(int32_t v,uint32_t q,uint16_t limit){
    uint32_t mag=(uint32_t)(v<0?-v:v);
    uint32_t x=(q*mag+512u)>>10;
    if(x>limit)x=limit;
    return (uint16_t)(OS_PWM_PERIOD-x);
}
bool os_current_step(os_pair feedback,os_pair target,int16_t sin_q14,int16_t cos_q14,
                     uint16_t gain_q11,uint16_t scale_raw,uint16_t compare_limit,
                     os_pwm_compare *out){
    os_pwm_inactive(out);
    if(scale_raw<745u || scale_raw>ADC_MAX || compare_limit>OS_PWM_PERIOD ||
       sin_q14< -16384 || sin_q14>16384 || cos_q14< -16384 || cos_q14>16384 ||
       feedback.a< -4095 || feedback.a>4095 || feedback.b< -4095 || feedback.b>4095 ||
       target.a< -8000 || target.a>8000 || target.b< -8000 || target.b>8000)return false;
    int32_t s=sin_q14,c=cos_q14;
    int32_t d=rounded(c*feedback.a+s*feedback.b,14);
    int32_t q=rounded(c*feedback.b-s*feedback.a,14);
    int32_t lim=(int32_t)(((uint32_t)scale_raw*972u+512u)>>10);
    d=clamp(rounded((target.a-d)*gain_q11,11),lim);
    q=clamp(rounded((target.b-q)*gain_q11,11),lim);
    int32_t a=rounded(c*d-s*q,14),b=rounded(s*d+c*q,14);
    uint32_t factor=(0x12c000u+scale_raw/2u)/scale_raw;
    uint16_t x=compare_value(a,factor,compare_limit),y=compare_value(b,factor,compare_limit);
    if(a<0)out->a1=x;else out->a0=x;
    if(b<0)out->b0=y;else out->b1=y;
    return true;
}
