.syntax unified
.cpu cortex-m0plus
.thumb
.section .firmware,"ax",%progbits
.global factory_image
factory_image:
#include "00_vectors.inc"
#include "01_application.inc"
#include "02_sine_table.inc"
#include "03_motion_uart.inc"
#include "04_startup_data.inc"
.org 0x7a44
