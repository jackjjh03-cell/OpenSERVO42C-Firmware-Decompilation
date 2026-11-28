.syntax unified
.thumb
.cpu cortex-m0plus
/* ARM EABI quotient/remainder register return conventions. */
.macro alias32 name, impl
.section .text.\name,"ax",%progbits
.global \name
.type \name,%function
.thumb_func
\name:
    push {r4,lr}
    bl \impl
    pop {r4,pc}
.size \name, .-\name
.endm
alias32 __aeabi_uidiv, os_udiv32_pair
alias32 __aeabi_uidivmod, os_udiv32_pair
alias32 __aeabi_idiv, os_idiv32_pair
alias32 __aeabi_idivmod, os_idiv32_pair
alias32 __aeabi_lmul, os_mul64
.macro div64 name, impl
.section .text.\name,"ax",%progbits
.global \name
.type \name,%function
.thumb_func
\name:
    push {r4,lr}
    sub sp,#24
    add r4,sp,#8
    str r4,[sp,#0]
    bl \impl
    ldm r4!,{r0-r3}
    add sp,#24
    pop {r4,pc}
.size \name, .-\name
.endm
div64 __aeabi_uldivmod, os_udiv64
div64 __aeabi_ldivmod, os_idiv64
