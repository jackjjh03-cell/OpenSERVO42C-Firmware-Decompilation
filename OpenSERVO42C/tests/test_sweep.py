import unittest
from studio import protocol as p
from studio.sweep import Sweep


class SweepTests(unittest.TestCase):
    def test_full_sweep(self):
        for direction in (-1,1):
            s=Sweep(8)
            for i in range(401):
                knot=i if i<=200 else 400-i
                raw=(16380+direction*(knot*16384//200))%16384
                self.assertFalse(s.accept((i,0,p.CAL_ACTIVE)))
                flags=(p.CAL_DONE if i==400 else p.CAL_ACTIVE)|p.CAL_READY
                self.assertEqual(s.accept((i,raw,flags)),i<400)
                self.assertFalse(s.accept((i,raw,flags)))
            self.assertEqual(len(s.lut),32768)
            self.assertEqual(s.peak,0)
    def test_bad_order_and_abort(self):
        s=Sweep(8)
        for status in ((1,0,3),(0,0,p.CAL_ABORTED),(0,16384,3),(0,0,6)):
            with self.assertRaises(ValueError): s.accept(status)
        self.assertEqual(s.samples,[])
    def test_stalled_and_wrong_direction(self):
        s=Sweep(8);s.accept((0,0,3));s.accept((1,82,3))
        with self.assertRaises(ValueError): s.accept((2,82,3))
        with self.assertRaises(ValueError): s.accept((2,40,3))
    def test_failed_closure(self):
        s=Sweep(8)
        for i in range(400):
            knot=i if i<=200 else 400-i
            s.accept((i,(knot*16384//200+(20 if i==200 else 0))%16384,3))
        with self.assertRaises(ValueError): s.accept((400,0,6))
        self.assertIsNone(s.lut)
