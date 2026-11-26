/* Application memory layout. Reserve the full identity page and all
 * persistent storage. */
ENTRY(reset_handler)
MEMORY {
 FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x7a00
 /* Explicit reserved regions: no output section may use these. */
 IDENTITY (r) : ORIGIN = 0x00007a00, LENGTH = 0x200
 SETTINGS (r) : ORIGIN = 0x00007c00, LENGTH = 0x200
 KNOTS (r) : ORIGIN = 0x00007e00, LENGTH = 0x200
 CALIBRATION (r) : ORIGIN = 0x00008000, LENGTH = 0x8000
 RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x1600
 EXTRA_RAM (rwx) : ORIGIN = 0x20001600, LENGTH = 0x0a00
}
SECTIONS {
 .vectors ORIGIN(FLASH) : { KEEP(*(.vectors)) } > FLASH
 .text : { *(.text .text.*) } > FLASH
 .rodata : { *(.rodata .rodata.*) } > FLASH
 .ARM.exidx : { *(.ARM.exidx*) } > FLASH
 .ARM.extab : { *(.ARM.extab*) } > FLASH
 __flash_code_end = ADDR(.ARM.exidx) + SIZEOF(.ARM.exidx) + SIZEOF(.ARM.extab);
 .work_ram ORIGIN(RAM) (NOLOAD) : { KEEP(*(.work_ram)) } > RAM
 .data : { __data_start = .; *(.data .data.*) __data_end = .; } > EXTRA_RAM AT>FLASH
 __data_load = LOADADDR(.data);
 .bss (NOLOAD) : { __bss_start = .; *(.bss .bss.*) *(COMMON) __bss_end = .; } > EXTRA_RAM
 __app_flash_end = SIZEOF(.data) ? LOADADDR(.data) + SIZEOF(.data) : __flash_code_end;
 __stack_top = 0x200015f8;
 __stack_floor = ADDR(.work_ram)+SIZEOF(.work_ram);
 __stack_budget = __stack_top-__stack_floor;
 /DISCARD/ : { *(.comment) *(.note*) }
 ASSERT(__stack_budget >= 0x800, "Less than 2 KiB legacy stack space")
 ASSERT(__stack_top < ORIGIN(EXTRA_RAM), "Stack overlaps extra globals")
 ASSERT(__bss_end <= ORIGIN(EXTRA_RAM)+LENGTH(EXTRA_RAM), "Globals exceed RAM")
 ASSERT(SIZEOF(.vectors) == 192, "Wrong vector size")
 ASSERT(SIZEOF(.work_ram) == 0xa54, "Legacy work RAM size changed")
 ASSERT(ADDR(.work_ram)+SIZEOF(.work_ram) <= __stack_top, "Work RAM collides with stack")
}
