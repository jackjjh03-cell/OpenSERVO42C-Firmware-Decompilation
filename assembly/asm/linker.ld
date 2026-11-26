ENTRY(factory_image)
MEMORY { FLASH (rx) : ORIGIN = 0, LENGTH = 0x7a44 }
SECTIONS {
 .firmware 0 : { KEEP(*(.firmware)) } > FLASH
 /DISCARD/ : { *(.comment*) *(.note*) *(.ARM.attributes*) }
 ASSERT(SIZEOF(.firmware) == 0x7a44, "Wrong original executable extent")
}
