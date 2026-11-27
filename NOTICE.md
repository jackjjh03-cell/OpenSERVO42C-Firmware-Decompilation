# Source notice

This repository is an unofficial reconstruction of the MKS SERVO42C V1.1.3 firmware for the HC32L130. It is not the original Makerbase source release and is not affiliated with or endorsed by the manufacturer. Original source names, comments, compiler settings and organization are not known.

The C implementation and the exact assembly reference are separate representations. The C build is independently compilable but not byte-identical to the factory executable. The assembly build reproduces the original application prefix. The normal Custom R02 build has been successfully programmed and booted on the original controller. The factory C build and the assembly reference have not been independently hardware-tested.

Project-authored code and documentation are offered under the MIT license. The license applies to material the project has the right to license; it does not grant rights to third-party firmware, graphics, tables or recovered data. Redistribution permissions for those vendor-derived materials have not been independently established.

Do not commit full flash backups, generated board images, identity data or calibration data.
