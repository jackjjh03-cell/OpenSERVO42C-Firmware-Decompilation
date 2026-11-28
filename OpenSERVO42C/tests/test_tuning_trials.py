import unittest
from studio.tuning_trials import make_trial, rank_trials

G = (4096, 0, 64, 2, 500)
C = ((16, 25), (1000, 1000), (2,))

def trace(actual=(0, 0, 25, 90, 105, 100, 100, 100, 100), demand=100, start=0, direction=1):
    return [((0xfffffffc+i*2)&0xffffffff,start+direction*a,start+direction*t,demand,200)
            for i,(a,t) in enumerate(zip(actual,(0,0,30,100,100,100,100,100,100)))]

def trial(rows=None, gains=G, context=C, start=0, end=100, faulted=False, tail=8):
    return make_trial(rows if rows is not None else trace(),2,faulted,gains,context,start,end,5,tail,500)

class TrialTests(unittest.TestCase):
    def test_forward_reverse_and_wrap(self):
        f=trial(); r=trial(trace(start=300,direction=-1),start=300,end=200)
        self.assertEqual(f.metrics,r.metrics)
        self.assertEqual(f.metrics['settling_ms'],2)
        self.assertEqual(f.metrics['overshoot_counts'],5)
    def test_truncated_or_wrong_endpoint(self):
        with self.assertRaises(ValueError): trial(trace()[:-4])
        with self.assertRaises(ValueError): trial(end=110)
        with self.assertRaises(ValueError): trial(tail=7)
    def test_fault_saturation_unsettled(self):
        with self.assertRaises(ValueError): trial(faulted=True)
        with self.assertRaises(ValueError): trial(trace(demand=500))
        with self.assertRaises(ValueError): trial(trace(actual=(0,0,10,20,30,40,50,60,70)))
    def test_rank_and_repeat(self):
        a=trial(); b=trial(trace(actual=(0,0,30,100,100,100,100,100,100)),gains=(4200,0,64,2,500))
        for p in ('Precision','Balanced','Speed','Low effort'):
            result=rank_trials([a,b,b],p)
            self.assertEqual(result[0][1],b.gains)
            self.assertEqual(result[0][2],2)
    def test_incomparable_trials(self):
        with self.assertRaises(ValueError): rank_trials([trial(),trial(context=((32,25),))])
        with self.assertRaises(ValueError): rank_trials([trial(),trial(gains=(4200,0,64,2,600))])
        with self.assertRaises(ValueError): rank_trials([trial(),trial(tail=6)])
    def test_irregular_samples(self):
        rows=trace(); rows[2]=(rows[2][0]+1,)+rows[2][1:]
        with self.assertRaises(ValueError): trial(rows)
    def test_reversed_target_after_endpoint(self):
        rows=trace(); rows[-2]=rows[-2][:2]+(99,)+rows[-2][3:]
        with self.assertRaises(ValueError): trial(rows)

    def test_late_departure_invalidates_trial(self):
        rows=trace();rows[-1]=(rows[-1][0],120,100,100,200)
        with self.assertRaisesRegex(ValueError,'after the observation window'):trial(rows)
