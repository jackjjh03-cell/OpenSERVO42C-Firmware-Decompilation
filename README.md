# OpenSERVO42C — Firmware Decompilation

[![Firmware build](https://github.com/jackjjh03-cell/OpenSERVO42C-Firmware-Decompilation/actions/workflows/firmware-build.yml/badge.svg?branch=main)](https://github.com/jackjjh03-cell/OpenSERVO42C-Firmware-Decompilation/actions/workflows/firmware-build.yml)

Source reconstruction of the MKS SERVO42C V1.1.3 closed-loop stepper motor firmware for the HC32L130 microcontroller.

This project started with a complete 64 KiB flash read from a SERVO42C running V1.1.3, obtained using the MCU's UART bootloader. The executable was examined and disassembled, then reconstructed as an independently buildable C application. The original instructions are used as the reference for checking the implementation.

The two goals of the project are to preserve the factory firmware and to provide a usable starting point for custom firmware development. The unmodified C source retains the original defaults, serial protocol, motor-control logic, menus and storage behavior as closely as the current reconstruction allows. Changes to factory behavior belong in a separate branch so the reference implementation remains available.

Three versions are included: a VS Code development project, a bare C source tree and a fixed-address assembly reference. The C versions build without the original HEX. The assembly reference reproduces the recovered original application bytes exactly.

> **Status:** Custom R02 has been flashed and is running on the original SERVO42C hardware. The reconstructed C also passes extensive host-side comparisons against the factory instructions. The factory C build is not byte-identical to the original HEX; the assembly reference is separate. Motor performance and compatibility with other boards still need verification.

## Custom firmware R02

[Browse test firmware](Test%20Firmware/) · [Changelog](CHANGELOG.md)

The current custom firmware adds SysInfo, RPM telemetry, encoder-fault settings access and UART diagnostics.

- [Custom R02 firmware](Test%20Firmware/R02/README.md)
- [SysInfo and UART diagnostic reference](Test%20Firmware/R02/SERVO42C_Custom_R02_Source/docs/SYSINFO.md)
- [PC configuration editor (Python)](Test%20Firmware/R02/SERVO42C_Custom_R02_Source/tools/config_editor.py)

## Contents

- [Hardware](#hardware)
- [Firmware overview](#firmware-overview)
- [Project layout](#project-layout)
- [Building the firmware](#building-the-firmware)
- [UART protocol](#uart-protocol)
- [Factory settings](#factory-settings)
- [Flash layout](#flash-layout)
- [Original HEX vs. reconstructed source](#original-hex-vs-reconstructed-source)
- [Verification](#verification)
- [Firmware images and hardware testing](#firmware-images-and-hardware-testing)
- [Current limitations](#current-limitations)
- [Custom firmware development](#custom-firmware-development)
- [Contributing](#contributing)
- [Attribution and licensing](#attribution-and-licensing)

## Hardware

| | |
|---|---|
| Controller | MKS SERVO42C |
| Reference firmware | V1.1.3 |
| Microcontroller | HC32L130 (Cortex-M0+) |
| Instruction set | ARM Thumb |
| Main flash | 64 KiB |
| Flash page size | 512 bytes |
| Source language | C |
| Build system | CMake / Ninja |
| Toolchain | Clang / LLD 17 |

The reconstruction is based on one firmware capture. Compatibility with other SERVO42C board revisions or firmware versions has not been established. It should not be assumed to work with the SERVO42D, SERVO57 or other related controllers.

## Firmware overview

The source covers the original firmware's main subsystems:

- Closed-loop position control and motor current regulation.
- Step/direction input and UART motion control.
- Encoder communication, calibration and position lookup.
- Motion state, origin return and protection handling.
- OLED display, menu navigation and telemetry.
- UART packet reception, command dispatch and response generation.
- Startup, clock setup, GPIO, timers, ADC and interrupt handlers.
- Nonvolatile settings, device identity and calibration storage.
- Fixed-point arithmetic, runtime helpers and initialized data.

There are 44 C source files, seven headers and a linker script. Display graphics, lookup data and menu strings required by the application are included in the source tree.

## Project layout

| Version | Contents | Use |
|---|---|---|
| [`development/`](development/) | C source, VS Code, CMake, build tools, tests and reference documentation | Editing, building and testing firmware |
| [`source/`](source/) | The same C files, headers and linker script without IDE dependencies | Using another editor or build environment |
| [`assembly/`](assembly/) | Fixed-address Thumb assembly and recovered data literals | Checking the original application at instruction and byte level |

```text
OpenSERVO42C-Firmware-Decompilation/
├── README.md
├── .gitignore
├── NOTICE.md
├── Test Firmware/
│   ├── README.md
│   └── R02/
├── development/
│   ├── .vscode/
│   ├── cmake/
│   ├── docs/
│   ├── factory/
│   │   ├── include/
│   │   ├── src/
│   │   ├── tests/
│   │   ├── layout.ld
│   │   └── build_audit.py
│   ├── tools/
│   ├── verification/
│   ├── SERVO42C.code-workspace
│   ├── CMakeLists.txt
│   └── NOTICE.md
├── source/
│   ├── include/
│   ├── src/
│   ├── layout.ld
│   └── README.md
└── assembly/
    ├── asm/
    │   ├── firmware.S
    │   └── linker.ld
    ├── REFERENCE_SHA256.txt
    ├── build.py
    ├── check_backup.py
    ├── BUILD.bat
    └── README.md
```

The two C distributions contain identical firmware files and produce the same linked application using the reference toolchain. The assembly is a separate reference, not a library linked into the C build. It excludes the board-specific identity, saved settings and calibration data.

### Main source files

| Area | Files |
|---|---|
| Startup and initialization | `reset.c`, `application_flow.c`, `board_startup.c`, `clock_and_bus.c` |
| Motor control | `motion_controller.c`, `motion_state.c`, `motor_math.c`, `current_feedback.c` |
| Encoder and calibration | `encoder.c`, `encoder_io.c`, `calibration_work.c` |
| Position and origin | `mode_and_position.c`, `origin.c` |
| Serial interface | `uart.c`, `commands.c`, `auxiliary_timer.c` |
| Display and menus | `display.c`, `menu.c`, `menu_navigation.c`, `telemetry.c`, `application_ui.c` |
| Flash storage | `settings.c`, `flash_driver.c`, `identity.c` |
| Hardware support | `board_io.c`, `board_timer_display.c`, `pin_config.c`, `peripherals.c` |
| Runtime and data | `arithmetic_runtime.c`, `runtime_memory.c`, `layout_assets.c`, `layout_startup_data.c` |

See [the firmware reference](development/docs/FIRMWARE.md) for the larger module list and implementation details. The [function traceability table](development/docs/function_traceability.csv) maps 270 original function entries to their corresponding reconstructed implementations.

## Building the firmware

### Automated builds

[GitHub Actions](https://github.com/jackjjh03-cell/OpenSERVO42C-Firmware-Decompilation/actions) builds the factory C source, normal R02, the profiling variant and the assembly reference when firmware files change. It also checks that the standalone factory C files match the development tree. These checks run on Linux with LLVM 17; no controller or private flash backup is required. Successful runs also provide application ELF files, linker maps, build reports and SHA-256 checksums under Artifacts. These are not complete board-programming images. Passing a build check does not replace testing on the motor.

### Requirements

- Clang/LLVM 17 with ARM target support, including `llvm-objdump`, `llvm-nm` and `llvm-objcopy`.
- LLD 17 (`ld.lld`).
- CMake 3.20 or newer.
- Ninja.
- Python 3.
- Visual Studio Code (optional, with the C/C++ extension for editing).

Make sure the command-line tools are available on `PATH`. The reference build was verified on Linux using Clang/LLD 17.

### Visual Studio Code

Open `development/SERVO42C.code-workspace` in VS Code, or open the `development/` folder using **File → Open Folder**.

Press **Ctrl+Shift+B** and run **Build firmware**. The configured task runs CMake and builds the application. To run the host tests, select **Tasks: Run Task → Run firmware tests (host)** from the command palette.

### Command line

From the repository root:

```sh
cd development
cmake -S . -B build/factory -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-clang.cmake
cmake --build build/factory
```

Leave `CMAKE_BUILD_TYPE` unset to retain the project's configured compiler flags. The application is compiled for `arm-none-eabi`, Cortex-M0+, Thumb, with `-Oz` and linked without a standard C runtime library.

Build output is placed in `development/build/factory/`:

| File | Description |
|---|---|
| `servo42c.elf` | Linked ARM application |
| `servo42c.map` | Linker map and symbol placement |
| `compile_commands.json` | Compilation database for editor tooling |
| `build_report.json` | Application size and hash |

The original firmware dump and a connected controller are not needed for a normal build. The ELF file is an application build, not a complete board programming image.

### Bare C and assembly builds

The bare C distribution can be compiled directly with Clang/LLD 17 for `arm-none-eabi`, Cortex-M0+, Thumb. Its [build instructions](source/README.md) give the full compiler and linker commands; all 44 C files must be included. No IDE, Python build utilities or original firmware dump are required.

The assembly reference can be rebuilt independently with Clang, LLD and LLVM objcopy. From the repository root:

```sh
cd assembly
mkdir -p build
clang --target=arm-none-eabi -mcpu=cortex-m0plus -mthumb -c asm/firmware.S -o build/reference.o
ld.lld -T asm/linker.ld -o build/reference.elf build/reference.o
llvm-objcopy -O binary --only-section=.firmware build/reference.elf build/reference.bin
```

The resulting 31,300-byte prefix should match the hash in [`assembly/REFERENCE_SHA256.txt`](assembly/REFERENCE_SHA256.txt). It is not a complete programming image. See [assembly build notes](assembly/README.md).

### Exact HEX reproduction

The assembly package also includes a Python build wrapper around Clang, LLD and LLVM objcopy. From `assembly/`, run:

```sh
python build.py
```

To reproduce the original complete 64 KiB capture privately, supply the original backup:

```sh
python build.py --backup /path/to/bin.hex --output private_build
```

The builder independently assembles the first 31,300 bytes, checks the reference hash and takes the remaining bytes from the original backup. It verifies full equality and checks the generated Intel HEX by decoding it. The original backup is required for this full-image operation and is not distributed with the project. This reproduces the original captured settings, not a factory reset. Intel HEX line formatting need not be identical when the flash bytes are identical.

The reference was freshly rebuilt and compared with the original capture: 31,300/31,300 application-prefix bytes and 65,536/65,536 full flash bytes matched. The full image includes the original private data and must not be published. No hardware programming is performed by the tool.

## UART protocol

MKS SERVO42C V1.1.3 (HC32L130)

This reference follows the V1.1.3 receiver and command dispatcher, not the protocol used by newer SERVO42D/57 controllers. Packet lengths, checksums, argument ranges and response handling were checked against the recovered instructions. Physical serial communication with the reconstructed firmware has not been tested. Some motion units and status meanings still require bench verification.

### Configuration

| Setting | Factory fallback | Options |
|---|---|---|
| Mode | `CR_vFOC` | `CR_OPEN`, `CR_vFOC`, `CR_UART` |
| UART baud | 0 (disabled) | 9600, 19200, 25000, 38400, 57600, 115200 |
| Address | `0xE0` | `0xE0`–`0xE9` |

Use the board's `UartBaud` and `UartAddr` menu options to enable communication. UART motor commands additionally require `CR_UART` mode. Match the host's baud rate to the board. For a first host-side test, 8 data bits, no parity and one stop bit (8N1) is a starting configuration, not a separately verified electrical/framing specification. Check the actual board's connector and signal levels before connecting an adapter.

The stock baud word is zero; valid saved records override the compiled defaults. The baud selection routine has an unusual behavior: index 0 is displayed as `Disable`, but selecting it stores 38400. A stored zero is still treated as disabled during UART initialization. The reconstruction retains this behavior.

### Packet format

Requests are raw binary bytes, not ASCII hex strings. There is no separate start byte or explicit length field. The first byte is the controller address.

```text
Request:   [address] [opcode] [optional arguments ...] [checksum]
Response:  [address] [data or status ...]               [checksum]
```

The checksum is the low eight bits of the sum of every preceding byte in the packet, including the address. Multi-byte command arguments and response data are sent most-significant byte first. Replies do not echo the opcode. The application does not define a universal response size; use the size appropriate to the command and controller state.

| Request length | Opcodes | Argument bytes |
|---:|---|---:|
| 3 | `30`–`3F` family; `F7` | 0 |
| 4 | `80`–`8F`, `90`–`9F` families; `F3`, `F6`, `FF` | 1 |
| 5 | `A0`–`AF` family | 2 |
| 8 | `FD` (finite motion) | 5 |

These are the receiver's length rules; only the 36 opcodes below have identified handlers. Unknown opcodes can be accepted by the length/checksum logic without having a valid action. The original receiver also has an unrestricted checksum check at length eight. A byte equal to the current address usually restarts reception, except within an in-progress `84`, `A*`, `F6` or `FD` request after the header. Bad checksums clear the receive count. While a packet is ready, further RX bytes are ignored. Send one request at a time and wait for its expected response or a timeout.

Most setting and motion handlers return a one-byte status (`01` accepted, `00` rejected or unavailable), followed by the checksum. Some queries use different values; calibration can return `02`, and origin requests can be accepted without an immediate acknowledgement. An accepted status is not proof of successful physical motion or flash programming.

#### Read-only packet examples

All numbers below are hexadecimal. Set the serial terminal to transmit binary hex bytes.

| Query | Request | Reply format |
|---|---|---|
| Encoder accumulator | `E0 30 10` | `E0` + six data bytes + checksum in closed-loop mode; status 0 in open-loop mode |
| Pulse/command position | `E0 33 13` | `E0` + four data bytes + checksum |
| Feedback position | `E0 36 16` | `E0` + four data bytes + checksum |
| Position error | `E0 39 19` | `E0` + two data bytes + checksum |
| Enable status | `E0 3A 1A` | `E0` + status + checksum |
| Protection/activity status | `E0 3E 1E` | `E0` + status + checksum |

For example, an enable response of `E0 01 E1` reports enabled; `E0 02 E2` reports disabled. A response such as `E0 00 00 00 05 E5` to a four-byte position query carries raw value 5. Values are illustrative, not actual measured responses.

A small packet helper for Python:

```python
def packet(address, opcode, *args):
    data = bytes((address, opcode, *args))
    return data + bytes((sum(data) & 0xFF,))


def valid_reply(data, address=0xE0):
    return len(data) >= 3 and data[0] == address and (sum(data[:-1]) & 0xFF) == data[-1]

print(packet(0xE0, 0x33).hex(' ').upper())  # E0 33 13
```

### Command reference

Request lengths include address, opcode and checksum. Reply lengths below refer only to the payload, excluding address and checksum. `u8`, `u16`, and `u32` indicate field width; `BE` indicates big-endian encoding. Signed position/error interpretations use the corresponding two's-complement width. Physical scaling is not assumed where unconfirmed.

#### Read and service commands

| Opcode | Request bytes | Operation | Reply / conditions |
|---|---:|---|---|
| `30` | 3 | Read encoder accumulator | 6 bytes (low 48 bits); 1-byte zero in open-loop mode |
| `33` | 3 | Read relative commanded/pulse position | 4 bytes; position minus stored reference |
| `36` | 3 | Read relative feedback position | 4 bytes; feedback minus stored reference |
| `39` | 3 | Read position error | 2 bytes; low 16 bits |
| `3A` | 3 | Read drive-enable state | 1 enabled, 2 disabled |
| `3D` | 3 | Request deferred protection-related action | 1 and sets a flag if active, otherwise 0; full semantics unconfirmed |
| `3E` | 3 | Read protection/activity flag | 1 active, 2 inactive |
| `3F` | 3 | Restore factory settings | Calls restore routine; status 1 if a reply is sent |

#### Configuration and calibration

| Opcode | Request bytes | Operation | Argument / behavior |
|---|---:|---|---|
| `80` | 4 | Calibrate | 0; closed-loop mode required; status 1/2 for routine result, 0 if rejected |
| `81` | 4 | Motor type | 0 = 0.9°, 1 = 1.8° |
| `82` | 4 | Operating mode | 0 = CR_OPEN, 1 = CR_vFOC, 2 = CR_UART |
| `83` | 4 | Current menu | Index 0–12; raw current = index × 200; half-current = index × 100 |
| `84` | 4 | Microsteps | Actual value, not menu index; 0 selects 256; custom values supported |
| `85` | 4 | Enable-input mode | 0 = L, 1 = H, 2 = Hold |
| `86` | 4 | Direction | 0 = CW, 1 = CCW |
| `87` | 4 | AutoSDD | 0 = disabled, 1 = enabled |
| `88` | 4 | Protection | 0 = disabled, 1 = enabled |
| `89` | 4 | Mplyer (interpolation) | 0 = disabled, 1 = enabled |
| `8A` | 4 | UART baud | Index 0–6; see table below |
| `8B` | 4 | UART address | Index 0–9; address = `E0` + index |
| `8C` | 4 | Acceleration menu | Index 0–7; raw value mapping below |

The OLED current menu contains more entries than the UART `83` handler accepts. UART indices 13–15 are rejected, despite corresponding entries in the display's option table. `84` treats zero as 256 and handles powers of two separately from custom microstep values. Menu callbacks can commit changes to flash and refresh runtime settings; repeated writes are not suitable for high-frequency control.

#### Baud and acceleration indices

| Index | UART baud label | Baud stored when selected | Acceleration raw value |
|---:|---|---:|---:|
| 0 | Disable | 38400 | 286 (menu label Disable) |
| 1 | 9600 | 9600 | 286 |
| 2 | 19200 | 19200 | 412 |
| 3 | 25000 | 25000 | 538 |
| 4 | 38400 | 38400 | 664 |
| 5 | 57600 | 57600 | 790 |
| 6 | 115200 | 115200 | 916 |
| 7 | — | Not accepted | 1042 |

When changing the baud or address, the old serial configuration may no longer receive a response. Do not assume the acknowledgement is always available on the old connection. Reconfigure the host and use a read-only query to check the new setting. Index 6 for baud produces the request `E0 8A 06 70` at address E0. This is a settings write, not a read-only probe.

#### Origin configuration

| Opcode | Request bytes | Operation | Argument / behavior |
|---|---:|---|---|
| `90` | 4 | Origin mode | 0 = disabled, 1 = DirMode, 2 = NearMode |
| `91` | 4 | Set origin | Argument byte required by receiver but ignored by handler; closed-loop and origin mode required |
| `92` | 4 | Origin speed | Index 0–4; factory default 2 |
| `93` | 4 | Origin direction | 0 = CW, 1 = CCW |
| `94` | 4 | Return to origin | Argument byte required but ignored; closed-loop, origin mode and valid stored origin required |

Accepted `91` and `94` requests set deferred flags without an immediate reply from the dispatcher. Rejected requests return status 0. Disabling origin mode invalidates the stored origin (`FFFF`). The speed index selects a delay setting; it should not be interpreted as a measured RPM.

#### Raw control parameters

| Opcode | Request bytes | Operation | Argument |
|---|---:|---|---|
| `A1` | 5 | Set Kp | u16 BE; factory default 1638 |
| `A2` | 5 | Set Ki | u16 BE; factory default 1 |
| `A3` | 5 | Set Kd | u16 BE; factory default 1638 |
| `A4` | 5 | Set acceleration | u16 BE; default 286 |
| `A5` | 5 | Set torque/current parameter | u16 BE; capped at 1200; default 1080 |

All five write the settings record and request a commit. The PID and current fields are raw firmware values; do not interpret them as physical gain or current without checking the control algorithm and board. Invalid tuning can cause unstable or unexpected motion. These commands are not required for initial serial testing.

#### UART motion commands

These commands are accepted only in `CR_UART` (mode 2).

| Opcode | Request bytes | Operation | Argument / behavior |
|---|---:|---|---|
| `F3` | 4 | UART enable control | 0 or 1; updates enable request and schedules mode refresh |
| `F6` | 4 | Continuous motion | Packed direction/speed byte; starts or updates continuous-motion state |
| `F7` | 3 | Stop | Calls factory STOP routine |
| `FD` | 8 | Finite motion | Packed direction/speed + four-byte unsigned BE count; relative target |
| `FF` | 4 | Save/clear continuous-motion configuration | `C8` saves speed/direction and changes motion state; `CA` clears saved motion and stops the timer. Requires continuous-motion state |

```text
F6: [addr] [F6] [direction/speed]                          [sum]
FD: [addr] [FD] [direction/speed] [count3] [count2] [count1] [count0] [sum]
```

Bit 7 of the packed field is direction; bits 0–6 are the raw speed (0–127). For `FD`, direction bit 1 adds the count to the current commanded position, while bit 0 subtracts it. Do not assume these directions are physical CW/CCW or that count equals full steps. The raw speed-to-RPM mapping is not established by hardware measurements. `FF` can change nonvolatile startup motion behavior: the original firmware supports restoring saved motion. Leave that feature unused until the startup behavior is understood. Do not test motion or calibration on an attached mechanism without independent power cutoff and safe mechanical clearance. A serial STOP command is not a hardware emergency stop.

### Source locations and verification

| File | Purpose |
|---|---|
| `factory/src/uart.c` | RX framing, checksum, interrupt and TX sequence |
| `factory/src/commands.c` | All 36 opcode handlers |
| `factory/src/menu.c` | Option mappings and settings commits |
| `factory/src/layout_startup_data.c` | OLED menu strings and option lists |
| `factory/src/board_io.c` | UART enable/disable and initialization |
| `factory/src/boot_services.c` | UART register configuration and baud divider |
| `factory/src/motion_controller.c` | Motor-control loop |
| `factory/src/auxiliary_timer.c` | Motion timer handling |
| `factory/include/defaults.h` | Stock fallback defaults |

Paths are relative to `development/`. The bare source tree contains the same files under `src/` and `include/`.

The recorded comparison suite exercised 10,952 dispatcher cases, 2,304 serial streams (17,408 receive interrupts), 1,424 menu callbacks and 128 STOP scenarios against the original machine instructions. Hardware, some callee effects and serial timing were modeled, not physically measured. For exact test boundaries see [validation notes](development/docs/VALIDATION.md) and [UART results](development/verification/uart_menu.txt).

## Factory settings

Fallback defaults are defined in [`development/factory/include/defaults.h`](development/factory/include/defaults.h). The settings loader uses the saved configuration when it is valid; it does not simply apply the compiled defaults every time the controller starts.

| Setting | Default |
|---|---|
| Control mode | `CR_vFOC` (1) |
| UART baud | Disabled (stored value 0) |
| UART address | `0xE0` |
| Microsteps | 16 |
| Direction | CW (0) |
| Interpolation | Enabled (1) |
| Protection | 0 |
| Kp | 1638 (raw) |
| Ki | 1 (raw) |
| Kd | 1638 (raw) |
| Current | 1200 (raw) |
| Half current | 600 (raw) |
| Torque | 1080 (raw) |
| Acceleration | 286 (raw) |

Raw values are the stored firmware constants; physical units and scaling should not be assumed where they have not been confirmed.

The reference controller was switched to `CR_UART` and 115200 baud before the firmware was read. Those changes are saved in flash and are not the original fallback defaults. The reconstructed source retains the stock `CR_vFOC` and disabled UART defaults.

One detail retained from the original firmware: the baud menu's option 0 is labeled Disable, but selecting it writes 38400; a stored baud of zero is reported as option 0. This is distinct from the actual startup default. See [Firmware reference](development/docs/FIRMWARE.md) for more on the settings loader and menu callbacks.

## Flash layout

```text
0x0000  +----------------------------------+
        | Application                      |  30.5 KiB
0x7A00  +----------------------------------+
        | Identity                         |  512 B
0x7C00  +----------------------------------+
        | Saved settings                   |  512 B
0x7E00  +----------------------------------+
        | Calibration knots / metadata     |  512 B
0x8000  +----------------------------------+
        | Encoder lookup table             |  32 KiB
        | 16,384 x 16-bit entries          |
0xFFFF  +----------------------------------+
```

| Address range | Size | Purpose |
|---|---:|---|
| `0x0000–0x79FF` | 31,232 B | C application region |
| `0x7A00–0x7BFF` | 512 B | Reserved identity page |
| `0x7C00–0x7DFF` | 512 B | Eight 64-byte settings records |
| `0x7E00–0x7FFF` | 512 B | Calibration knots and validity metadata |
| `0x8000–0xFFFF` | 32,768 B | Encoder lookup table |

The original executable contains compressed startup data extending through `0x7A43`. The C implementation relocates initialized data and reserves the entire identity page, so the linker has a slightly different application boundary. The original board's identity, configuration and calibration must be kept with that board.

## Original HEX vs. reconstructed source

The reference HEX is a full flash capture, not just executable code. It includes settings and calibration data, so comparing the size of the HEX text file with the C build would not be meaningful. The following comparisons use decoded flash bytes and the linked application.

### Application size

```text
Original application prefix  31,300 B  |████████████████████████████████|
Reconstructed C application  30,428 B  |███████████████████████████████░|
```

The original prefix includes its compressed startup data up to `0x7A43`. The C size is the loaded application extent before padding. These are related but not identical linker regions.

```text
C application region (31,232 B)
|███████████████████████████████████████░| 97.43% used

Used: 30,428 B       Free: 804 B
```

### Binary comparison

The compiled C and original HEX were compared at the same flash addresses across `0x0000–0x79FF` (31,232 bytes, including erased padding).

```text
Matching bytes      986 / 31,232  ( 3.16%) |█░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░|
Different bytes  30,246 / 31,232  (96.84%) |███████████████████████████████░|
```

This low same-address match is expected with a different compiler, function placement, data layout and generated instructions. It is not a measure of how much of the firmware was recovered or how accurately the C behaves.

A separate fixed-address assembly reference was also reconstructed and reassembled:

```text
Original application prefix       31,300 B
Assembly reference matching       31,300 B |████████████████████████████████| 100%
```

The assembly result establishes byte identity over the original application prefix (31,300 bytes), not over a publicly distributed full 64 KiB image. It does not establish byte identity for the C source. The original compiler, optimization settings, source names and comments cannot be recovered with certainty from the HEX alone.

### Reference build

| Metric | Result |
|---|---:|
| C source modules | 44 |
| Original function entries mapped | 270 |
| External C declarations audited | 281 |
| Unresolved linker symbols | 0 |
| Application extent | 30,428 B |
| Free space before reserved flash | 804 B |
| C build vs. companion bare-source build | Identical |

Application SHA-256 (padded with `0xFF` to `0x7A00`):

```text
9c0922b0aedabd35c9b22596dd5e2902c3c63e9b1f0c4293bda754a3a381b4c7
```

Reference factory application SHA-256 (same address range):

```text
1a64209c9cb46679f78628b8eb82cddcb5ef78972b9f6699a9b41aeb12610595
```

The build hash allows source and toolchain changes to be checked against the reference build. A different hash after editing is normal.

## Verification

Testing was performed using the original ARM instructions and the independently compiled C application. The host-side Thumb model compares register and workspace state, ordered peripheral writes, call arguments, serial packets and other observable behavior at defined checkpoints. Some peripheral responses and external calls are scripted or mocked.

The following are selected recorded test groups. Counts are not additive: some tests share scenarios or are included in broader regression runs.

| Test group | Recorded result |
|---|---:|
| UART dispatch, including all 36 known opcodes | 10,952 cases passed |
| UART receiver | 2,304 streams / 17,408 RX interrupts |
| Menu callbacks | 1,424 cases passed |
| Startup and integration regression | 790 scenarios passed |
| Complete board initialization | 72 cases passed |
| Reset to identity check | 24 cases passed |
| Ordered register writes across the two preceding startup groups | 29,628 matched |
| Startup diagnostics | 32 cases passed |
| Foreground UI / protection | 256 cases passed |
| Motion timer interrupt | 96 cases passed |
| Current ADC interrupt | 96 cases passed |
| STOP motion state | 128 cases passed |
| Differential settings commits | 64 cases passed |
| Differential flash primitives | 8 cases passed |
| Origin return | 96 cases passed |
| Complete calibration sweep | 8 scripted sweeps passed |
| Calibration lookup writer | 16,384 halfword writes |
| Arithmetic and telemetry | 1,216 checks passed |

The calibration sweeps use scripted encoder input. The lookup writer test uses modeled NOR flash. UART dispatch and receive tests use defined mock boundaries; they are not physical serial or motor tests. Arithmetic checks include original 64-bit division evaluated against a mathematical reference.

The latest source cleanup was rebuilt and verified against the same application hash. Earlier behavioral test logs apply to that binary; not every test was rerun after formatting changes.

Recorded results are available under [`development/verification/`](development/verification/). For the test methodology, caveats and additional commands, see [Firmware validation](development/docs/VALIDATION.md).

### Running the tests

Run the following from `development/` after building the application.

```sh
cmake --build build/factory --target verify
python factory/build_audit.py --persistence
python factory/build_audit.py --edit-smoke
python tools/check_stock_defaults.py
python tools/check_c_declarations.py
python tools/check_traceability.py build/factory/servo42c.elf
```

Comparisons against the original instructions require a local, matching original flash backup:

```sh
python tools/audit_factory.py build/factory/servo42c.elf /path/to/bin.hex --full --extra
python tools/final_factory_checks.py build/factory/servo42c.elf /path/to/bin.hex
python tools/runtime_parity.py build/factory/servo42c.elf /path/to/bin.hex
```

Do not run the Python tools with assertions disabled. The original controller's full flash dump is not included. The 1,576-byte `development/factory/tests/original_initialized_ram.bin` is a recovered startup-data fixture used by the host tests, not a controller backup. The recorded test logs include configuration values used in the settings-replay tests. Keep the original backup and all generated images outside the repository.

## Firmware images and hardware testing

The normal Custom R02 build has been successfully programmed and booted on the original controller. Complete board-specific HEX images are not distributed here. The offline exporter can assemble an application and a matching, same-board backup into a development HEX or BIN while preserving `0x7A00–0xFFFF`. It checks the firmware revision and build report, but cannot establish that an image is safe to program or run. It also preserves the backup's saved user settings; it does not automatically reset them to stock defaults.

Before programming a controller, establish a working recovery method, verify the programmer's erase behavior, retain the original backup and complete the checks in [Hardware validation](development/factory/BENCH_VALIDATION.md). Do not copy identity or calibration data between boards.

## Current limitations

- The reconstructed C does not produce a byte-identical copy of the original application. A separate assembly reference reproduces the original application prefix exactly.
- Normal R02 startup was confirmed on the original controller before the project-name change in the Credits screen; the updated build has not been independently hardware-tested. Physical testing of the unmodified factory C build, motor regulation, current sensing, calibration and loaded operation remains outstanding.
- Real-time interrupt latency, nested interrupt stack use and electrical behavior have not been established by the host tests.
- The original firmware's flash wrappers do not propagate all programming errors. That behavior is retained for compatibility; power-loss recovery is not guaranteed.
- Some names, internal data types and raw parameter meanings are inferred from the executable rather than recovered from the original source.
- Other board revisions and firmware versions have not been validated.

Changes that intentionally alter factory behavior should be maintained separately from the reference implementation, so the original behavior remains available for comparison.

## Custom firmware development

### Custom Firmware R02

Custom firmware based on the V1.1.3 reconstruction, featuring SysInfo diagnostics, live RPM monitoring, improved magnet error handling and extended UART functionality.

- [Custom R02 Firmware Documentation](Test%20Firmware/R02/SERVO42C_Custom_R02_Source/README.md)
- [SysInfo Diagnostic and UART Reference](Test%20Firmware/R02/SERVO42C_Custom_R02_Source/docs/SYSINFO.md)


The factory C source is the reference implementation. Start custom changes from a separate branch and keep the source-only distribution synchronized with the development tree. Useful entry points are `defaults.h` for fallback settings, `commands.c` and `uart.c` for the serial interface, `menu.c` for the display menu and `motion_controller.c` for the motor-control logic. Changes to saved settings, timing or control parameters require additional validation; a successful build is not enough to establish safe motor operation.

## Contributing

Corrections are most useful when they include the relevant board and firmware version, the affected function or flash address, and the expected and observed behavior. For source changes, include a build result and a relevant regression test where possible.

Keep board backups, device-specific settings and calibration files out of the repository. Do not post private flash captures in issues or pull requests.

## Attribution and licensing

MKS SERVO42C and the original firmware are associated with Makerbase. This project is an independent reconstruction and is not affiliated with or endorsed by the manufacturer. The reconstructed code, names and organization should not be mistaken for the manufacturer's original source release.

Project-authored code and documentation are available under the [MIT license](LICENSE). See [NOTICE.md](NOTICE.md) for the status of recovered vendor material.

Thank you Makerbase for keeping us waiting for 5 years!!!
