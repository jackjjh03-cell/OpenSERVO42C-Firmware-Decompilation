#include <stdint.h>
#include <assert.h>
#include <stdio.h>
uint64_t os_udiv32_pair(uint32_t,uint32_t);
uint64_t os_idiv32_pair(int32_t,int32_t);
void os_udiv64(uint64_t,uint64_t,uint64_t *);
void os_idiv64(uint64_t,uint64_t,uint64_t *);
uint64_t os_mul64(uint64_t,uint64_t);
static uint64_t seed=0x27d4eb2f165667c5ULL;
static uint64_t rnd(void){seed^=seed<<13;seed^=seed>>7;seed^=seed<<17;return seed;}
static void check(uint64_t a,uint64_t b){
    uint64_t r[2];os_udiv64(a,b,r);
    assert(r[0]==(b?a/b:0)&&r[1]==(b?a%b:a));
    assert(os_mul64(a,b)==a*b);
    int64_t x=(int64_t)a,y=(int64_t)b;
    if(y && !(x==INT64_MIN&&y==-1)){os_idiv64(a,b,r);assert(r[0]==(uint64_t)(x/y)&&r[1]==(uint64_t)(x%y));}
    uint32_t m=(uint32_t)a,n=(uint32_t)b;
    uint64_t p=os_udiv32_pair(m,n);
    assert((uint32_t)p==(n?m/n:0)&&(uint32_t)(p>>32)==(n?m%n:m));
    int32_t i=(int32_t)m,j=(int32_t)n;
    if(j && !(i==INT32_MIN&&j==-1)){p=os_idiv32_pair(i,j);assert((uint32_t)p==(uint32_t)(i/j)&&(uint32_t)(p>>32)==(uint32_t)(i%j));}
}
int main(void){
    const uint64_t e[]={0,1,2,UINT32_MAX,UINT64_C(1)<<32,INT64_MAX,UINT64_C(1)<<63,UINT64_MAX-1,UINT64_MAX};
    for(unsigned i=0;i<sizeof e/sizeof e[0];i++)for(unsigned j=0;j<sizeof e/sizeof e[0];j++)check(e[i],e[j]);
    for(unsigned i=0;i<100000;i++){uint64_t a=rnd(),b=rnd();check(a,b);check(a,(uint32_t)b);}
    for(unsigned i=0;i<65536u;i++){
        check(i,745u+i%3351u);check(UINT32_MAX-i,1u+i%2047u);
        check((UINT64_C(1)<<63)+i,UINT64_C(1)<< (i%64u));
    }
    puts("runtime arithmetic passed");return 0;
}
