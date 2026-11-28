#include "factory_layout.h"
extern unsigned char __stack_top;
extern unsigned char __data_start[], __data_end[], __data_load[], __bss_start[], __bss_end[];
void initialize_c_globals(void) {
    for (uint8_t *p=__data_start, *q=__data_load;p<__data_end;) *p++=*q++;
    for (uint8_t *p=__bss_start;p<__bss_end;) *p++=0;
}
extern void factory_system_init(void), application_main(void);
void default_handler(void) {
    for (;;) {
    }
}
void hardfault_handler(void) {
    for (;;) {
    }
}
/* Preserve factory ordering: system init before scatter copy, then application main. */
void reset_handler(void) {
    RAM32(0xe000ed08u)=0;
    factory_system_init();
    initialize_c_globals();
    initialize_factory_data();
    application_main();
    for (;;) {
    }
}
extern void auxiliary_timer_irq(void);
extern void current_adc_irq(void);
extern void empty_045c(void);
extern void empty_0494(void);
extern void empty_049c(void);
extern void empty_04a4(void);
extern void empty_04ac(void);
extern void empty_04b4(void);
extern void empty_0548(void);
extern void empty_0550(void);
extern void empty_05e8(void);
extern void empty_05f0(void);
extern void empty_05f2(void);
extern void empty_05fa(void);
extern void empty_0602(void);
extern void empty_060a(void);
extern void empty_0612(void);
extern void empty_061a(void);
extern void empty_0622(void);
extern void empty_062a(void);
extern void empty_0632(void);
extern void empty_063a(void);
extern void empty_063c(void);
extern void empty_0644(void);
extern void empty_0cc4(void);
extern void empty_106c(void);
extern void empty_14b0(void);
extern void empty_1570(void);
extern void empty_16e4(void);
extern void enable_input_irq(void);
extern void motion_timer_irq(void);
#ifdef SYSINFO_PROFILE
extern void sysinfo_profile_irq(void);
#endif
extern void startup_adc_irq(void);
extern void uart0_irq(void);
__attribute__((section(".vectors"), used)) const uintptr_t vectors[48]={
(uintptr_t)&__stack_top,
(uintptr_t)reset_handler,
(uintptr_t)default_handler,
(uintptr_t)hardfault_handler,
0,
0,
0,
0,
0,
0,
0,
(uintptr_t)default_handler,
0,
0,
(uintptr_t)default_handler,
(uintptr_t)empty_16e4,
(uintptr_t)empty_05e8,
(uintptr_t)enable_input_irq,
(uintptr_t)empty_05f0,
(uintptr_t)empty_05f2,
(uintptr_t)current_adc_irq,
#ifdef SYSINFO_PROFILE
(uintptr_t)sysinfo_profile_irq,
#else
(uintptr_t)motion_timer_irq,
#endif
(uintptr_t)uart0_irq,
(uintptr_t)empty_063a,
(uintptr_t)empty_04a4,
(uintptr_t)empty_04ac,
(uintptr_t)empty_0602,
(uintptr_t)empty_060a,
(uintptr_t)empty_14b0,
(uintptr_t)empty_1570,
(uintptr_t)empty_0612,
(uintptr_t)empty_061a,
(uintptr_t)auxiliary_timer_irq,
(uintptr_t)empty_049c,
(uintptr_t)empty_0622,
(uintptr_t)empty_062a,
(uintptr_t)empty_0632,
(uintptr_t)empty_0548,
(uintptr_t)empty_0cc4,
(uintptr_t)empty_05fa,
(uintptr_t)startup_adc_irq,
(uintptr_t)empty_0550,
(uintptr_t)empty_063c,
(uintptr_t)empty_0644,
(uintptr_t)empty_04b4,
(uintptr_t)empty_0494,
(uintptr_t)empty_106c,
(uintptr_t)empty_045c,
};
