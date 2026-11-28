import struct
import unittest
from tools.export_hex import application_bytes, intel_hex, FLASH_END, STACK_TOP


def elf(segments, entry=193):
    """Construct minimal ELF32 program headers independently of the linker."""
    ident=b'\x7fELF\x01\x01\x01'+bytes(9)
    head=struct.pack('<HHIIIIIHHHHHH',2,40,1,entry,52,0,0,52,32,len(segments),0,0,0)
    headers=[];body=bytearray()
    for typ,virt,phys,payload,memsize in segments:
        headers.append(struct.pack('<8I',typ,52+32*len(segments)+len(body),virt,phys,len(payload),memsize,5,4))
        body.extend(payload)
    return ident+head+b''.join(headers)+body


def vectors(stack=STACK_TOP, reset=193):
    return struct.pack('<II',stack,reset)+bytes(184)+b'\x00\xbf'


class ExportTests(unittest.TestCase):
    def test_flash_and_ram_initializers(self):
        data=application_bytes(elf([(1,0,0,vectors(),194),(1,0x20000000,0x1000,b'AB',2),(1,0x20000002,0x20000002,b'',10)]))
        self.assertEqual(bytes(data[i] for i in (0x1000,0x1001)),b'AB')
        self.assertNotIn(194,data)
        self.assertNotIn(0x20000000,data)
        decoded={}
        for line in intel_hex(data).splitlines():
            raw=bytes.fromhex(line[1:]);self.assertEqual(sum(raw)&255,0)
            self.assertEqual(len(raw),raw[0]+5)
            if raw[3]==0:
                addr=int.from_bytes(raw[1:3],'big')
                decoded.update((addr+i,b) for i,b in enumerate(raw[4:-1]))
        self.assertEqual(decoded,data)
        self.assertTrue(intel_hex(data).endswith(':00000001FF\n'))
    def test_reject_reserved_and_overlapping(self):
        for extra in ((1,FLASH_END,FLASH_END,b'X',1),(1,100,100,b'X',1),(1,0x20000000,0x20000000,b'X',1),(1,FLASH_END-1,FLASH_END-1,b'XX',2)):
            with self.assertRaises(ValueError):application_bytes(elf([(1,0,0,vectors(),194),extra]))
    def test_invalid_vectors_and_truncated(self):
        for bad in (vectors(stack=0),vectors(reset=192),vectors(reset=201),vectors()[:8]):
            with self.assertRaises(ValueError):application_bytes(elf([(1,0,0,bad,len(bad))]))
        with self.assertRaises(ValueError):application_bytes(elf([(1,0,0,vectors(),194)])[:-1])
        with self.assertRaises(ValueError):application_bytes(b'no')
        with self.assertRaises(ValueError):intel_hex({FLASH_END:0})
