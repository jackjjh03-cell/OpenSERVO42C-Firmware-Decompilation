import unittest
from decimal import Decimal
from studio.units import scale,readout,command_counts

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

    def test_motion_conversion(self):
        self.assertEqual(command_counts('180','Degrees'),32768)
        self.assertEqual(command_counts('-0.25','Revolutions'),-16384)
        self.assertEqual(command_counts('1','Millimeters','8'),8192)
        self.assertEqual(command_counts('0.5','Inches','25.4'),32768)
        self.assertEqual(command_counts('0.00274658203125','Degrees'),1)
        self.assertEqual(command_counts('-0.00274658203125','Degrees'),-1)
        self.assertEqual(command_counts('0','Millimeters','8'),0)
        self.assertEqual(command_counts('-2147483648','Counts'),-2147483648)
        self.assertEqual(command_counts('2147483647','Counts'),2147483647)

    def test_motion_rejects_invalid_values(self):
        for value in ('NaN','Infinity','1e999999999','wrong','0.0000000001',''):
            with self.assertRaises(ValueError):command_counts(value,'Degrees')
        for value in ('1.5','2147483648','-2147483649'):
            with self.assertRaises(ValueError):command_counts(value,'Counts')
        with self.assertRaises(ValueError):command_counts('1','Millimeters','')
        with self.assertRaises(ValueError):command_counts('16','Revolutions',minimum=-1000000,maximum=1000000)
        with self.assertRaises(ValueError):command_counts('-16','Revolutions',minimum=-1000000,maximum=1000000)
