# Firmware source

This directory contains the Custom R02 firmware source. The legacy workspace and reserved flash layout are unchanged. The standard build does not instrument the motor interrupt. `SYSINFO_PROFILE=ON` adds experimental profiling. Build with the parent CMake project and use its `verify` target. The baseline traceability file describes the earlier factory reconstruction, not the relocated custom application.
