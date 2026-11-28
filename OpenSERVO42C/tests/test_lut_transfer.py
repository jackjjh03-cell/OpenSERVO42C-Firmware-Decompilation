import struct
import unittest
import zlib
from studio import protocol as p
from studio.lut_transfer import LutTransfer

LUT=struct.pack('<16384H',*((4*i+100)&65535 for i in range(16384)))

class Fake:
    def __init__(self):
        self.flags=0; self.loaded=True; self.active=False; self.offset=0; self.verified=False; self.writes=[]; self.fail=False
    def run_status(self):return {'flags':self.flags}
    def read(self,op):
        if op==p.GET_STORAGE:return (self.loaded,0,1)
        if op==p.LUT_STATUS:return (self.offset,int(self.active),p.LUT_VERIFIED if self.verified else p.LUT_DIRTY if self.active else p.LUT_LEGACY)
        raise AssertionError(op)
    def write(self,op,*args):
        self.writes.append((op,args))
        if self.fail:raise IOError('lost reply')
        if op==p.LUT_BEGIN:
            assert args==(zlib.crc32(LUT)&0xffffffff,);self.active=True
        elif op==p.LUT_WRITE:
            assert args==(self.offset,LUT[2*self.offset:2*self.offset+32]);self.offset+=16
        elif op==p.LUT_FINISH:
            assert self.offset==16384;self.active=False;self.verified=True
        else:raise AssertionError(op)

class TransferTests(unittest.TestCase):
    def test_full_ordered_transfer(self):
        c=Fake();t=LutTransfer(LUT)
        self.assertEqual(t.advance(c),(0,False))
        for i in range(1024):self.assertEqual(t.advance(c),(32*(i+1),False))
        self.assertEqual(t.advance(c),(32768,True));self.assertEqual(len(c.writes),1026)
        with self.assertRaises(ValueError):t.advance(c)
    def test_no_write_while_enabled_or_pending(self):
        for attr,value in [('flags',1),('flags',2),('loaded',False),('active',True)]:
            c=Fake();setattr(c,attr,value);t=LutTransfer(LUT)
            with self.assertRaises(ValueError):t.advance(c)
            self.assertEqual(c.writes,[])
    def test_no_retry(self):
        c=Fake();t=LutTransfer(LUT);t.advance(c);c.fail=True
        with self.assertRaises(IOError):t.advance(c)
        self.assertEqual(t.offset,0)
        with self.assertRaises(ValueError):t.advance(c)
        self.assertEqual(len(c.writes),2)
    def test_invalid_lut(self):
        for data in (b'',b'\x00'*32768,LUT[:-2]):
            with self.assertRaises(ValueError):LutTransfer(data)
