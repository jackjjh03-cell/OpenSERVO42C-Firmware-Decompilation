# SERVO42C firmware — V1.1.3

Embedded C source for the HC32L130 (ARM Cortex-M0+). Includes motor control, encoder calibration, step/direction and UART interfaces, OLED menus and flash settings storage.

## Source layout

- `src/` — 44 C translation units
- `include/` — shared types, hardware interfaces, default configuration and memory definitions
- `layout.ld` — application linker script

## Build

Use Clang/LLD 17 targeting `arm-none-eabi` with Thumb instructions. Include every C source file and use the supplied linker script without a standard C library or startup library. Example for a POSIX shell (run in a clean directory):

```sh
mkdir -p build
for f in src/*.c; do
  clang --target=arm-none-eabi -mcpu=cortex-m0plus -mthumb -Oz \
    -ffunction-sections -fdata-sections -ffreestanding -fno-builtin \
    -fno-strict-aliasing -Wall -Wextra -Werror -Iinclude \
    -c "$f" -o "build/$(basename "$f" .c).o" || exit 1
done
ld.lld --gc-sections --no-undefined -T layout.ld -Map=build/servo42c.map \
  -o build/servo42c.elf build/*.o
```

Default configuration: CR_vFOC, UART disabled, address `0xE0`, 16 microsteps. Edit `include/defaults.h` to change fallback settings; valid saved settings override these values.

The application uses flash `0x0000–0x79FF`; preserve the device identity, settings and calibration pages at `0x7A00–0xFFFF`. The reference build occupies 30,428 bytes, leaving 804 bytes. This package contains no IDE, tests, generated firmware or board backup.

See [NOTICE.md](NOTICE.md) for source status and validation limitations. Do not program the controller before establishing a safe bench validation and recovery procedure. The companion development project includes the image exporter and host tests.
