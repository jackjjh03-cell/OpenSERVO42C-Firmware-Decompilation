#include <stdint.h>
#include <stddef.h>
/* Freestanding arithmetic. Divisor zero returns quotient 0, remainder n;
 * callers must reject it before using the result. */
uint64_t os_udiv32_pair(uint32_t n,uint32_t d){
    uint32_t q=0;
    if(d)for(unsigned i=32;i--;){if((n>>i)>=d){n-=d<<i;q|=1u<<i;}}
    return ((uint64_t)n<<32)|q;
}
uint64_t os_idiv32_pair(int32_t n,int32_t d){
    uint32_t a=(uint32_t)n,b=(uint32_t)d;
    uint64_t pair=os_udiv32_pair(n<0?0u-a:a,d<0?0u-b:b);
    uint32_t q=(uint32_t)pair,r=(uint32_t)(pair>>32);
    if((n<0)!=(d<0))q=0u-q;
    if(n<0)r=0u-r;
    return ((uint64_t)r<<32)|q;
}
void os_udiv64(uint64_t n,uint64_t d,uint64_t *out){
    uint64_t q=0,r=0;
    if(!d){out[0]=0;out[1]=n;return;}
    for(unsigned i=0;i<64;i++){
        uint64_t bit=n>>63; n<<=1;
        uint64_t carry=r>>63;r=(r<<1)|bit;q<<=1;
        if(carry||r>=d){r-=d;q|=1u;}
    }
    out[0]=q;out[1]=r;
}
void os_idiv64(uint64_t n,uint64_t d,uint64_t *out){
    unsigned neg_n=(unsigned)(n>>63),neg_d=(unsigned)(d>>63);
    os_udiv64(neg_n?0u-n:n,neg_d?0u-d:d,out);
    if(neg_n!=neg_d)out[0]=0u-out[0];
    if(neg_n)out[1]=0u-out[1];
}
/* Multiplication modulo 2^64; four 16-bit products avoid recursive EABI calls. */
uint64_t os_mul64(uint64_t a,uint64_t b){
    uint32_t x=(uint32_t)a,y=(uint32_t)b;
    uint32_t x0=x&65535u,x1=x>>16,y0=y&65535u,y1=y>>16;
    uint32_t lo=x0*y0, mid=x1*y0+(lo>>16);
    uint32_t hi=x1*y1+(mid>>16);
    mid=(mid&65535u)+x0*y1;
    hi+=(mid>>16)+(uint32_t)(a>>32)*y+x*(uint32_t)(b>>32);
    return ((uint64_t)hi<<32)|((mid<<16)|(lo&65535u));
}
#ifdef __arm__
void __aeabi_memcpy(void *dst,const void *src,size_t n){
    volatile uint8_t *d=dst;const volatile uint8_t *s=src;
    while(n--)*d++=*s++;
}
void __aeabi_memcpy4(void *d,const void *s,size_t n){__aeabi_memcpy(d,s,n);}
void __aeabi_memcpy8(void *d,const void *s,size_t n){__aeabi_memcpy(d,s,n);}
void __aeabi_memclr(void *dst,size_t n){volatile uint8_t *d=dst;while(n--)*d++=0;}
void __aeabi_memclr4(void *d,size_t n){__aeabi_memclr(d,n);}
void __aeabi_memclr8(void *d,size_t n){__aeabi_memclr(d,n);}
#endif
