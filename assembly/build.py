#!/usr/bin/env python3
"""Reassemble the exact application; optionally reconstruct the private full capture."""
import argparse
import hashlib
from pathlib import Path
import shutil
import subprocess
import tempfile
from check_backup import read_backup

ROOT = Path(__file__).resolve().parent
PREFIX_SIZE = 0x7A44
PREFIX_SHA = '6538f476ee297aac4bc265b53e21392f6fd56ccb4f52339f2f112aaebfb22e56'
CAPTURE_SHA = 'a17a42ad07816278b51dbe750d4527881b5b564f5d687449248464e19266809a'

def sha(data):
    return hashlib.sha256(data).hexdigest()

def intel_hex(data):
    def record(address, kind, payload):
        raw = bytes([len(payload)]) + address.to_bytes(2, 'big') + bytes([kind]) + payload
        return ':' + (raw + bytes([-sum(raw) & 255])).hex().upper()
    return '\n'.join([record(i, 0, data[i:i+16]) for i in range(0, len(data), 16)] + [record(0, 1, b'')]) + '\n'

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--backup', type=Path, help='Original private 64 KiB bin.hex or BIN; required for complete capture')
    parser.add_argument('--output', type=Path, default=ROOT/'build', help='New output directory')
    args = parser.parse_args()
    out = args.output.resolve()
    if out.exists():
        parser.error('Output directory already exists; choose a new directory.')
    tools = {}
    for name in ('clang', 'ld.lld', 'llvm-objcopy'):
        tools[name] = shutil.which(name)
        if not tools[name]:
            parser.error('Missing tool on PATH: ' + name)
    backup = read_backup(args.backup) if args.backup else None
    if backup is not None and sha(backup) != CAPTURE_SHA:
        parser.error('Backup does not match the original captured flash hash.')
    with tempfile.TemporaryDirectory() as directory:
        tmp = Path(directory)
        def run(name, *arguments):
            subprocess.run([tools[name], *map(str, arguments)], cwd=ROOT, check=True)
        run('clang', '--target=arm-none-eabi', '-mcpu=cortex-m0plus', '-mthumb', '-c', ROOT/'asm/firmware.S', '-o', tmp/'reference.o')
        run('ld.lld', '-T', ROOT/'asm/linker.ld', '-o', tmp/'reference.elf', tmp/'reference.o')
        run('llvm-objcopy', '-O', 'binary', '--only-section=.firmware', tmp/'reference.elf', tmp/'reference.bin')
        prefix = (tmp/'reference.bin').read_bytes()
        if len(prefix) != PREFIX_SIZE or sha(prefix) != PREFIX_SHA:
            raise ValueError('Reassembled application differs from exact reference.')
        full = None
        if backup is not None:
            full = prefix + backup[PREFIX_SIZE:]
            if full != backup or sha(full) != CAPTURE_SHA:
                raise ValueError('Full image mismatch.')
            (tmp/'roundtrip.hex').write_text(intel_hex(full), encoding='ascii')
            if read_backup(tmp/'roundtrip.hex') != backup:
                raise ValueError('HEX round-trip mismatch.')
        out.mkdir(parents=True, exist_ok=False)
        shutil.copyfile(tmp/'reference.elf', out/'reference.elf')
        (out/'reference.bin').write_bytes(prefix)
        (out/'reference_prefix.hex').write_text(intel_hex(prefix), encoding='ascii')
        report = f'Application prefix: {len(prefix)} bytes\nSHA-256: {sha(prefix)}\nExact reference match: PASS\n'
        if full is not None:
            (out/'private_capture.bin').write_bytes(full)
            (out/'private_capture.hex').write_text(intel_hex(full), encoding='ascii')
            report += f'Full capture: {len(full)} bytes\nSHA-256: {sha(full)}\nDifferent bytes: 0\nHEX round-trip: PASS\n'
        report += 'Offline verification only; not a stock-reset image or hardware programming authorization.\n'
        (out/'build_report.txt').write_text(report, encoding='ascii')
        print(report)

if __name__ == '__main__':
    main()
