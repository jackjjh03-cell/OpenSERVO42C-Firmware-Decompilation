# Test Firmware

Published custom firmware revisions for the MKS SERVO42C, based on the V1.1.3 reconstruction for the HC32L130.

| Revision | Status | Contents |
|---|---|---|
| [R02](R02/) | Current; flashed and running on the original controller | Source, build instructions, SysInfo reference and PC configuration utility |

Each revision has its own directory so previous releases remain available when new firmware is published. Compiled HEX files may be included when suitable for distribution. Full board-specific flash captures, identity and encoder calibration data are not distributed. See each revision's flashing instructions before programming a controller.

The [factory firmware reconstruction](../README.md) is maintained separately. [Automated builds](https://github.com/jackjjh03-cell/OpenSERVO42C-Firmware-Decompilation/actions) check compilation; they do not replace hardware testing.

## Build downloads

Open a successful [Firmware build](https://github.com/jackjjh03-cell/OpenSERVO42C-Firmware-Decompilation/actions/workflows/firmware-build.yml) run and download the `servo42c-applications` artifact (GitHub sign-in may be required). It contains separate factory, normal R02 and R02 profiling application builds, linker maps, build reports and SHA-256 checksums. These are development outputs, not full flash images. See [flashing instructions](R02/SERVO42C_Custom_R02_Source/FLASHING.md) for same-board backup requirements.

## Compatibility

| Controller | Status |
|---|---|
| SERVO42C, HC32L130, original V1.1.3 capture | Normal R02 flashed and booted |
| Other SERVO42C board revisions | Not verified |
| SERVO42D / SERVO57 | Not supported by this release |

See the [changelog](../CHANGELOG.md) for revision history.
