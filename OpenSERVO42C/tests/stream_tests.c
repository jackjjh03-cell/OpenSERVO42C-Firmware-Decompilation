#include "stream.h"
#include <assert.h>
#include <stdio.h>
int main(void){
    os_stream s;os_stream_reset(&s);
    assert(!os_stream_feed(&s,0x01,0));assert(!os_stream_feed(&s,0xa5,1));
    assert(!os_stream_feed(&s,0xa5,2));
    const uint8_t frame[]={0x5a,1,8,2,0,0,0};
    for(unsigned i=0;i<sizeof frame;i++)assert(os_stream_feed(&s,frame[i],3+i)==(i==sizeof frame-1?8u:0u));
    assert(s.bytes[0]==0xa5&&s.bytes[1]==0x5a&&s.bytes[3]==8);
    assert(!os_stream_feed(&s,0xa5,UINT32_MAX-50u));
    assert(!os_stream_feed(&s,0x5a,50));assert(s.used==0);
    const uint8_t bad[]={0xa5,0x5a,1,0,1,49};
    for(unsigned i=0;i<sizeof bad;i++)assert(!os_stream_feed(&s,bad[i],100+i));
    assert(s.used==0);
    for(unsigned i=0;i<56;i++){
        uint8_t v=i==0?0xa5:i==1?0x5a:i==2?1:i==5?48:0xa5;
        assert(os_stream_feed(&s,v,200+i)==(i==55?56u:0u));
    }
    uint32_t rng=1;for(unsigned i=0;i<100000;i++){
        rng=rng*1664525u+1013904223u;
        size_t n=os_stream_feed(&s,(uint8_t)(rng>>24),300+i);
        assert(s.used<56&&n<=56);
    }
    puts("stream tests passed");return 0;
}
