# Source notice

This is an unofficial C reconstruction of the MKS SERVO42C V1.1.3 firmware for the HC32L130, based on a captured binary. It is not a Makerbase source release. Names, comments and source organization are reconstructed; the original compiler and build options are unknown.

The application is independently buildable, but its compiled instructions are not byte-identical to the vendor executable. Normal R02 has been successfully programmed and booted on the original controller. Host tests do not establish full motor operation, exact interrupt timing or flash power-loss recovery. Project-authored code and documentation are available under MIT; third-party material is subject to its own rights. No vendor license or endorsement is implied.
