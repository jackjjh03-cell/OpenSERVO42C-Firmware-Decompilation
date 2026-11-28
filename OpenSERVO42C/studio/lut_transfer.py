"""One acknowledged transaction at a time; never automatically retry a flash write."""
import struct
import zlib
from . import protocol as p


class LutTransfer:
    def __init__(self, lut):
        self.lut = bytes(lut)
        if len(self.lut) != 32768:
            raise ValueError("Calibration LUT must contain exactly 16384 halfwords")
        values = struct.unpack('<16384H', self.lut)
        deltas = [(b-a+32768)%65536-32768 for a,b in zip(values,values[1:]+values[:1])]
        if abs(sum(deltas)) != 65536 or -32768 in deltas or (min(deltas)<0<max(deltas)):
            raise ValueError("Invalid calibration table winding or direction")
        self.crc = zlib.crc32(self.lut) & 0xffffffff
        self.offset = 0
        self.started = self.finished = self.failed = False

    def advance(self, client):
        """Run exactly one step. Returns acknowledged bytes, completion flag."""
        if self.failed or self.finished:
            raise ValueError("Transfer is no longer active; inspect controller before restarting")
        try:
            if not self.started:
                if client.run_status()['flags'] & 3:
                    raise ValueError("Disable motor output and external EN before programming")
                if not client.read(p.GET_STORAGE)[0]:
                    raise ValueError("Save drive configuration first")
                if client.read(p.LUT_STATUS)[1]:
                    raise ValueError("Previous transfer active; abort it explicitly first")
                client.write(p.LUT_BEGIN, self.crc)
                self.started = True
            elif self.offset < len(self.lut):
                client.write(p.LUT_WRITE, self.offset//2, self.lut[self.offset:self.offset+32])
                self.offset += 32
            else:
                client.write(p.LUT_FINISH)
                if client.read(p.LUT_STATUS) != (16384,0,p.LUT_VERIFIED):
                    raise ValueError("Controller did not report verified calibration")
                self.finished = True
        except Exception:
            self.failed = True
            raise
        return self.offset, self.finished
