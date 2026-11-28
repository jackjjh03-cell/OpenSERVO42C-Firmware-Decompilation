#include "stream.h"
/* Feed from one owner only; a foreground parser must not race a UART ISR. */
void os_stream_reset(os_stream *s){s->used=0;s->last_ms=0;}
size_t os_stream_feed(os_stream *s,uint8_t byte,uint32_t now_ms){
    if(s->used && (uint32_t)(now_ms-s->last_ms)>=OS_STREAM_TIMEOUT_MS)s->used=0;
    s->last_ms=now_ms;
    if(s->used==0){if(byte==0xa5u)s->bytes[s->used++]=byte;return 0;}
    if(s->used==1){
        if(byte==0x5au)s->bytes[s->used++]=byte;
        else if(byte!=0xa5u)s->used=0;
        return 0;
    }
    s->bytes[s->used++]=byte;
    if((s->used==3 && byte!=1u)||(s->used==6 && byte>48u)){
        s->used=0;if(byte==0xa5u)s->bytes[s->used++]=byte;return 0;
    }
    if(s->used>=6u && s->used==8u+s->bytes[5]){
        size_t n=s->used;s->used=0;return n;
    }
    return 0;
}
