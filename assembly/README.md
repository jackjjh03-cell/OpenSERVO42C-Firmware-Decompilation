# SERVO42C V1.1.3 — Exact assembly build

This package reassembles the recovered ARM Thumb instructions and data at their original flash addresses. It is separate from the editable C implementation. It is not the original Makerbase compiler or source code.

Requirements: Python 3.8+, Clang 17 with ARM support, LLD 17 and LLVM objcopy on PATH. The reference toolchain was tested on Linux. No serial connection or programming takes place.

## Build the application reference

```sh
python build.py
```

Outputs `build/reference.elf`, `build/reference.bin`, `build/reference_prefix.hex` and a report. The verified application prefix is 31,300 bytes (`0x0000–0x7A43`), including the original compressed startup data. It is incomplete as a board programming image.

## Reproduce the complete original capture (private)

```sh
python build.py --backup /path/to/bin.hex --output private_build
```

Supply the original complete 64 KiB backup. The builder validates its SHA-256, assembles the prefix independently, copies the remaining bytes from the supplied backup, checks full equality and decodes the newly written HEX to verify the round trip. The full output is `private_capture.hex` and `private_capture.bin`.

This reproduces the captured flash contents, including the user's saved UART configuration and calibration. It does not reset settings or generate an image from C. Intel HEX record formatting may differ from the input file even when every decoded flash byte is identical.

Do not publish the private backup or generated full capture. Do not program these outputs without a verified recovery procedure and appropriate hardware checks. Redistribution rights for vendor-derived assembly and data remain unconfirmed.
