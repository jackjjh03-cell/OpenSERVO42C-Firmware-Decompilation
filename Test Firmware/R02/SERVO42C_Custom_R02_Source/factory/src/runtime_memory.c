#include "factory_layout.h"
/* forward copy, word transfers only when both pointers are aligned.
 * The original leaves the advanced destination in r0. r1/r2 are scratch. */
uint32_t runtime_copy_forward(uint32_t dst, uint32_t src, uint32_t n) {
    if (!((dst|src)&3u))while (n>=4) {
        RAM32(dst)=RAM32(src);
        dst+=4;
        src+=4;
        n-=4;
    }
    while (n--) {
        RAM8(dst++)=RAM8(src++);
    }
    return dst;
}
/* memset-like wrapper, original destination returned. */
uint32_t runtime_memset(uint32_t dst, uint32_t value, uint32_t n) {
    uint32_t orig=dst;
    while (n--)RAM8(dst++)=(uint8_t)value;
    return orig;
}
/* startup scatter zero. Linker supplies word-aligned size. */
void scatter_zero_words(uint32_t unused, uint32_t dst, uint32_t n) {
    (void)unused;
    while (n) {
        RAM32(dst)=0;
        dst+=4;
        n-=4;
    }
}
/* ARM EABI memory copy support. Do not call libc or compiler-generated
 * memcpy recursively. */
void __aeabi_memcpy(void *destination, const void *source, size_t count) {
    volatile uint8_t *d=destination;
    const volatile uint8_t *s=source;
    while (count--)*d++=*s++;
}
/* ARM EABI aligned memory clear support. */
void __aeabi_memclr4(void *destination, size_t count) {
    volatile uint8_t *d=destination;
    while (count--)*d++=0;
}
