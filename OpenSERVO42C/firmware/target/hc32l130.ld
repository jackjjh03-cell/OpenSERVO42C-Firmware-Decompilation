/* Independent firmware layout. Keep all original persistent pages reserved. */
ENTRY(reset_handler)
MEMORY {
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x7600
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x2000
}
__flash_target = 0x7000; /* 28 KiB; 1.5 KiB code headroom before settings slots */
__settings_slot0 = 0x7600;
__settings_slot1 = 0x7800;
__ram_static_target = 0x1000; /* 4 KiB review target, not a hard limit */
__stack_top = ORIGIN(RAM) + LENGTH(RAM);
__stack_floor = __stack_top - 0x800;
SECTIONS {
  .vectors ORIGIN(FLASH) : { KEEP(*(.vectors .isr_vector)) } > FLASH
  .text : { *(.text .text.*) *(.rodata .rodata.*) } > FLASH
  .data : ALIGN(8) {
    __data_start = .; *(.data .data.*) . = ALIGN(8); __data_end = .;
  } > RAM AT> FLASH
  __data_load = LOADADDR(.data);
  .bss (NOLOAD) : ALIGN(8) {
    __bss_start = .; *(.bss .bss.*) *(COMMON) . = ALIGN(8); __bss_end = .;
  } > RAM
  .noinit (NOLOAD) : ALIGN(8) { *(.noinit .noinit.*) . = ALIGN(8); __static_end = .; } > RAM
  .stack __stack_floor (NOLOAD) : { . = . + 0x800; } > RAM
  __app_flash_end = MAX(ADDR(.text)+SIZEOF(.text), LOADADDR(.data)+SIZEOF(.data));
  __static_bytes = __static_end - ORIGIN(RAM);
  /* Freestanding C/assembly, no exception unwinding. Keep real payload
   * checks independent of ARM unwind table generation/merging. */
  /DISCARD/ : { *(.comment) *(.note*) *(.ARM.extab*) *(.ARM.exidx*) }
  ASSERT(SIZEOF(.vectors) == 192, "Expected 48 vector entries")
  __flash_review_required = (__app_flash_end > __flash_target);
  ASSERT(__app_flash_end <= ORIGIN(FLASH)+LENGTH(FLASH), "Application overlaps reserved flash")
  __ram_static_headroom = __stack_floor - __static_end;
  __ram_review_required = (__static_bytes > __ram_static_target);
  ASSERT(__static_end <= __stack_floor, "Static RAM overlaps stack")
}
