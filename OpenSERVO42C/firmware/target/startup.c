#include <stdint.h>
/* Core startup only. Board-specific initialization and IRQs are separate. */
extern uint8_t __stack_top, __data_start[], __data_end[], __data_load[], __bss_start[], __bss_end[];
extern void os_target_main(void);
extern void os_target_panic(void);
void default_handler(void){
    __asm volatile("cpsid i" ::: "memory");
    os_target_panic();
    for(;;)__asm volatile("wfi");
}
void reset_handler(void){
    __asm volatile("cpsid i" ::: "memory");
    *(volatile uint32_t *)(uintptr_t)0xe000ed08u=0u;
    for(uint8_t *d=__data_start,*s=__data_load;d<__data_end;)*d++=*s++;
    for(uint8_t *d=__bss_start;d<__bss_end;)*d++=0;
    os_target_main();
    default_handler();
}
#define WEAK_IRQ(name) void name(void) __attribute__((weak,alias("default_handler")))
WEAK_IRQ(enable_irq);WEAK_IRQ(current_irq);WEAK_IRQ(position_irq);WEAK_IRQ(uart_irq);
WEAK_IRQ(aux_irq);WEAK_IRQ(startup_adc_irq);WEAK_IRQ(systick_irq);
#define D ((uintptr_t)default_handler)
#define H(x) ((uintptr_t)(x))
/* IRQ indices from original V1.1.3 reset.c: 1,4,5,6,16,24. */
__attribute__((section(".vectors"),used)) const uintptr_t vectors[48]={
    H(&__stack_top),H(reset_handler),D,D,0,0,0,0,0,0,0,D,0,0,D,H(systick_irq),
    D,H(enable_irq),D,D,H(current_irq),H(position_irq),H(uart_irq),D,
    D,D,D,D,D,D,D,D,H(aux_irq),D,D,D,D,D,D,D,H(startup_adc_irq),D,D,D,D,D,D,D
};
