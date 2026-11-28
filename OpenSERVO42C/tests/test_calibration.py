import struct
import unittest
from studio.calibration import merge_sweeps, generate_lut, validate_sweep


def sweep(direction=1, origin=16380):
    return [(origin+direction*(i*16384//200))%16384 for i in range(200)]


class CalibrationTests(unittest.TestCase):
    def test_both_directions_and_wrap(self):
        for direction in (-1,1):
            knots=sweep(direction)
            lut=struct.unpack('<16384H',generate_lut(knots))
            for i,k in enumerate(knots):
                self.assertEqual(lut[k],(i*65536+100)//200)
            d=[(b-a+32768)%65536-32768 for a,b in zip(lut,lut[1:]+lut[:1])]
            self.assertEqual(sum(d),direction*65536)
            self.assertTrue(all(x*direction>=0 for x in d))
    def test_midpoints(self):
        a=sweep();b=[(v+8)%16384 for v in a]
        merged,peak=merge_sweeps(a,b,8)
        self.assertEqual(peak,8)
        self.assertEqual(merged,tuple((v+4)%16384 for v in a))
        self.assertEqual(len(generate_lut(merged)),32768)
        with self.assertRaises(ValueError): merge_sweeps(a,b,7)
    def test_bad_sweeps(self):
        a=sweep();a[10]=a[9]
        for invalid in (a,a[:-1],[0]*200,[16384]*200):
            with self.assertRaises(ValueError): generate_lut(invalid)
        a=sweep();a[2],a[3]=a[3],a[2]
        with self.assertRaises(ValueError): validate_sweep(a)
        with self.assertRaises(ValueError): merge_sweeps(sweep(),sweep(-1),100)
    def test_irregular_knot_spacing(self):
        knots=[(v+(i%7)-3)%16384 for i,v in enumerate(sweep())]
        lut=struct.unpack('<16384H',generate_lut(knots))
        self.assertEqual(lut[knots[100]],32768)
