#include <stdint.h>
/* Integer arithmetic support for Cortex-M0+. Division returns the quotient
 * in the low word and the remainder in the high word. Avoid division and
 * 64-bit multiplication operators here to prevent recursive runtime calls. */
uint64_t unsigned_divmod_pair(uint32_t n, uint32_t d) {
    uint32_t q=0;
    for (unsigned i=32;i--;) {
        if ((n>>i)>=d) {
            n-=d<<i;
            q|=1u<<i;
        }
    }
    return ((uint64_t)n<<32)|q;
}
uint64_t signed_divmod_pair(int32_t n, int32_t d) {
    uint32_t a=(uint32_t)n, b=(uint32_t)d;
    uint64_t pair=unsigned_divmod_pair(n<0?0u-a:a, d<0?0u-b:b);
    uint32_t q=(uint32_t)pair, r=(uint32_t)(pair>>32);
    if ((n<0)!=(d<0))q=0u-q;
    if (n<0)r=0u-r;
    return ((uint64_t)r<<32)|q;
}
uint32_t factory_divide(uint32_t n, uint32_t d) {
    return (uint32_t)unsigned_divmod_pair(n, d);
}
int32_t factory_signed_divide(int32_t n, int32_t d) {
    return (int32_t)(uint32_t)signed_divmod_pair(n, d);
}
uint64_t factory_multiply64(uint64_t a, uint64_t b) {
    uint64_t product=0;
 /* Shift/add is defined modulo 2^64, including signed bit patterns. */
    while (b) {
        if (b&1u)product+=a;
        a<<=1;
        b>>=1;
    }
    return product;
}
