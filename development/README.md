# SERVO42C firmware — V1.1.3

Embedded motor-control firmware for the HC32L130 (ARM Cortex-M0+). Provides closed-loop stepper control, encoder calibration, step/direction input, serial control, OLED menus and nonvolatile settings.

## Build

Requirements: Clang/LLVM 17 with ARM target support, LLD, llvm-objdump, llvm-nm, CMake 3.20+, Ninja and Python 3. Put the tools on PATH.

Open this directory in Visual Studio Code and run **Build firmware** (Ctrl+Shift+B), or use the command line:

```sh
cmake -S . -B build/factory -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-clang.cmake
cmake --build build/factory
```

Leave `CMAKE_BUILD_TYPE` unset to use the configured size-optimized flags. The build creates `servo42c.elf`, `servo42c.map`, `compile_commands.json` and `build_report.json` in `build/factory/`. No board connection or binary backup is needed to compile.

## Source tree

| Directory | Contents |
|---|---|
| `factory/src/` | Application, motor control, hardware drivers, UI and runtime |
| `factory/include/` | Interfaces, default settings and memory layout |
| `factory/layout.ld` | Linker memory regions and stack constraints |
| `factory/tests/` | Host-side firmware tests |
| `cmake/` | Cross-compiler configuration |
| `tools/` | Image inspection, export and compatibility tests |
| `docs/` | Firmware reference and validation notes |
| `verification/` | Recorded test results |

## Configuration

Default settings are defined in `factory/include/defaults.h`. The control mode is CR_vFOC, UART is disabled (baud 0), UART address is `0xE0`, and microsteps are 16. Valid saved settings take precedence over defaults. See [firmware reference](docs/FIRMWARE.md) for source modules and flash layout.

## Test and image export

```sh
cmake --build build/factory --target verify
python factory/build_audit.py --persistence
```

Further tests and baseline build information: [validation](docs/VALIDATION.md).

The offline image exporter requires a full backup from the same physical controller and a matching firmware revision. It retains the reserved flash pages; it does not program the controller. See [hardware validation](factory/BENCH_VALIDATION.md) before any bench testing. No prebuilt firmware image is supplied.

## Source status

Unofficial reconstructed firmware; not a hardware-qualified vendor release. See [NOTICE.md](NOTICE.md). The companion IDE-independent source package contains the same C, header and linker files.
