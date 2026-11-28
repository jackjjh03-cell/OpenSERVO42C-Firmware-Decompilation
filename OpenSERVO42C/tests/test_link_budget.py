"""Check the target linker limits with synthetic ARM objects (not firmware)."""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
LINKER = ROOT / 'firmware' / 'target' / 'hc32l130.ld'

class LinkBudgetTests(unittest.TestCase):
    def link(self, text=64, data=16, bss=64, noinit=0):
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp) / 'fixture.s'
            obj = Path(tmp) / 'fixture.o'
            elf = Path(tmp) / 'fixture.elf'
            src.write_text('''.syntax unified
.thumb
.section .vectors,"a",%progbits
.space 192
.section .text,"ax",%progbits
.global reset_handler
.thumb_func
reset_handler:
b .
.space {text}
.section .data,"aw",%progbits
.space {data}
.section .bss,"aw",%nobits
.space {bss}
.section .noinit,"aw",%nobits
.space {noinit}
'''.format(text=text, data=data, bss=bss, noinit=noinit), encoding='ascii')
            subprocess.run([os.environ.get('ARM_CLANG', 'clang'), '--target=arm-none-eabi',
                            '-mcpu=cortex-m0plus', '-mthumb', '-c', str(src), '-o', str(obj)], check=True)
            return subprocess.run([os.environ.get('ARM_LLD', 'ld.lld'), '-T', str(LINKER),
                                   str(obj), '-o', str(elf)], capture_output=True, text=True)

    def test_within_budget(self):
        result = self.link()
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_empty_initialized_data(self):
        result = self.link(data=0)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_flash(self):
        result = self.link(text=0x7600)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('Application overlaps reserved flash', result.stderr)

    def test_data_flash(self):
        result = self.link(text=0x7400, data=512)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('Application overlaps reserved flash', result.stderr)

    def test_flash_above_review_target(self):
        result = self.link(text=0x7100)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_above_review_target(self):
        result = self.link(bss=5120)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_static_ram(self):
        result = self.link(bss=6144)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('Static RAM overlaps stack', result.stderr)

    def test_noinit_ram(self):
        result = self.link(bss=6000, noinit=256)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('Static RAM overlaps stack', result.stderr)

if __name__ == '__main__':
    unittest.main()
