import unittest
from decimal import Decimal
from studio.units import scale,readout

class UnitTests(unittest.TestCase):
    def test_full_turn(self):
        self.assertEqual(scale('Degrees')*65536,360)
        self.assertEqual(scale('Revolutions')*65536,1)
        self.assertEqual(scale('Millimeters','8')*65536,8)
        self.assertEqual(scale('Inches','25.4')*65536,1)
    def test_signed_and_fractional(self):
        self.assertEqual(readout(-32768,65536,8192,'Millimeters','8'), 'Actual -4.0000 mm | Target 8.0000 mm | Speed 1.0000 mm/s')
        self.assertIn('Actual -180.0000 deg',readout(-32768,0,0,'Degrees'))
    def test_bad_lead(self):
        for lead in ('','NaN','Infinity','-1','0','1e100','1e-100','wrong'):
            with self.assertRaises(ValueError):scale('Millimeters',lead)
        with self.assertRaises(ValueError):scale('Bogus')
