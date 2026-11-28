import unittest
from studio.tuning import analyze

class TuningTests(unittest.TestCase):
    def test_forward_reverse_and_wrap(self):
        rows=[((0xfffffffe+i*2)&0xffffffff,a,t,10,100) for i,(a,t) in enumerate([(0,0),(10,30),(70,100),(110,100),(100,100),(100,100)])]
        a=analyze(rows,2,tolerance=5)
        self.assertEqual(a['overshoot_counts'],10)
        self.assertEqual(a['settling_ms'],4)
        self.assertEqual(a['peak_error_counts'],30)
        self.assertEqual(a,analyze([(t,-x,-y,-d,l) for t,x,y,d,l in rows],2,tolerance=5))
    def test_reject_bad_trace(self):
        rows=[(i*2,i*10,i*10,0,0) for i in range(6)]
        with self.assertRaises(ValueError): analyze(rows,2)
        with self.assertRaises(ValueError): analyze(rows,2,True)
        with self.assertRaises(ValueError): analyze(rows[1:],3)
    def test_unsettled(self):
        rows=[(i*2,a,t,0,0) for i,(a,t) in enumerate([(0,0),(0,100),(0,100),(50,100),(70,100),(80,100)])]
        self.assertIsNone(analyze(rows,2,tolerance=5)['settling_ms'])
