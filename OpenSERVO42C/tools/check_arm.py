"""Compile every ARM module and check unresolved symbols. Not a flash image."""
import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
FIRMWARE = ROOT / 'firmware'
ALLOWED = {'__aeabi_ldivmod', '__aeabi_lmul', '__aeabi_memclr',
           '__aeabi_memclr4', '__aeabi_memclr8', '__aeabi_memcpy',
           '__aeabi_memcpy4', '__aeabi_memcpy8', '__aeabi_uidiv',
           '__aeabi_uidivmod', '__aeabi_idiv', '__aeabi_idivmod',
           '__aeabi_uldivmod'}

def main():
    with tempfile.TemporaryDirectory() as temp:
        out = Path(temp)
        objects = []
        for group in ('core', 'hal', 'motion', 'communication'):
            for src in sorted((FIRMWARE / group).glob('*.c')):
                obj = out / (group + '_' + src.stem + '.o')
                subprocess.run([os.getenv('ARM_CLANG', 'clang'), '--target=arm-none-eabi',
                                '-mcpu=cortex-m0plus', '-mthumb', '-Oz', '-ffreestanding',
                                '-fno-builtin', '-fomit-frame-pointer', '-ffunction-sections', '-fdata-sections',
                                '-Wall', '-Wextra', '-Werror', '-Wconversion', '-Wsign-conversion',
                                '-I', str(FIRMWARE / 'include'), '-c', str(src), '-o', str(obj)], check=True)
                objects.append(obj)
        combined = out / 'combined.o'
        subprocess.run([os.getenv('ARM_LLD', 'ld.lld'), '-r', *map(str, objects), '-o', str(combined)], check=True)
        nm = subprocess.check_output([os.getenv('ARM_NM', 'nm'), '-u', str(combined)], text=True)
        symbols = {line.split()[-1] for line in nm.splitlines() if line.strip()}
        if symbols - ALLOWED:
            raise SystemExit('Unexpected unresolved symbols: ' + ', '.join(sorted(symbols - ALLOWED)))
        print('Compiled and relocatably linked {} Cortex-M0+ modules'.format(len(objects)), flush=True)
        print('ARM runtime symbols still required: ' + ', '.join(sorted(symbols)), flush=True)
        subprocess.run([os.getenv('ARM_SIZE', 'size'), str(combined)], check=True)

if __name__ == '__main__':
    main()
