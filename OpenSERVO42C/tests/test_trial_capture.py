import unittest
from studio import protocol as p
from studio.trial_capture import begin_trial, finish_trial, inspect_trial, apply_trial_gains

class Fake:
    def __init__(self):
        self.writes=[]; self.active=True; self.tick=0xfffffffb; self.flags=8; self.trace=(0,0,0,8)
        self.settings={p.GET_PID:(4096,0,64,2,500),p.GET_LIVE_PID:(0,4096,0,64,2,500),p.GET_DRIVE:(16,1000,500,2048,300,25,0,600,0),p.GET_MOTION:(10000,30000,15000,0,0),p.GET_CONFIG:(10000,10000,500,800,10,20,2),p.GET_LIMITS:(-1000,1000,1),p.GET_CONTROL:(0,0,0,0)}
        self.rows=[((0xfffffffc+i*2)&0xffffffff,a,t,100,200) for i,(a,t) in enumerate(zip((0,0,25,90,105,100,100,100,100),(0,0,30,100,100,100,100,100,100)))]
    def read(self,op): return self.trace if op==p.TRACE_STATUS else self.settings[op]
    def status(self): return dict(state=2 if self.active else 0,mode=1,faults=0,observed=0,warnings=0,position_lost=0,actual=0,target=0,speed_counts_s=0)
    def run_status(self): return dict(flags=47 if self.active else 44,pending_fault=0,tick=self.tick)
    def write(self,op,*args):
        self.writes.append((op,args))
        if op==p.TRACE_START:
            self.trace=(1,args[0],args[1],9);self.tick=0xfffffffd
    def finish(self): self.active=False;self.trace=(9,9,2,8)
    def read_trace(self): return self.rows,2,False

class CaptureTests(unittest.TestCase):
    def begin(self,c): return begin_trial(c,100,9,2,5,8)
    def test_success(self):
        c=Fake(); cap=self.begin(c)
        self.assertEqual(c.writes,[(p.TRACE_START,(9,2)),(p.MOVE_ABS,(100,))])
        c.finish();t=finish_trial(c,cap);self.assertEqual(t.metrics['settling_ms'],2)
    def test_no_move_without_limits_or_protection(self):
        for op,value in [(p.GET_LIMITS,(-1000,1000,0)),(p.GET_CONFIG,(10000,10000,500,800,10,20,1)),(p.GET_LIVE_PID,(1,4096,0,64,2,500))]:
            c=Fake();c.settings[op]=value
            with self.assertRaises(ValueError): self.begin(c)
            self.assertFalse(c.writes)
    def test_old_firmware_and_out_of_range(self):
        c=Fake();c.trace=(0,0,0,0)
        with self.assertRaises(ValueError): self.begin(c)
        c=Fake()
        with self.assertRaises(ValueError): begin_trial(c,1001,9,2,5,8)
        self.assertFalse(c.writes)
    def test_interrupted_or_changed(self):
        c=Fake();cap=self.begin(c);c.finish();c.trace=(9,9,2,12)
        with self.assertRaises(ValueError): finish_trial(c,cap)
        c.trace=(9,9,2,8);c.settings[p.GET_PID]=(5000,0,64,2,500)
        with self.assertRaises(ValueError): finish_trial(c,cap)
    def test_stale_capture(self):
        c=Fake();cap=self.begin(c);c.finish();c.rows=[((t+100)&0xffffffff,a,b,d,l) for t,a,b,d,l in c.rows]
        with self.assertRaises(ValueError): finish_trial(c,cap)
    def test_cancelled_before_trace_or_motion(self):
        c=Fake()
        with self.assertRaisesRegex(ValueError,'cancelled'):
            begin_trial(c,100,9,2,5,8,cancelled=lambda:True)
        self.assertEqual(c.writes,[])
        with self.assertRaisesRegex(ValueError,'cancelled'):
            begin_trial(c,100,9,2,5,8,cancelled=lambda:bool(c.writes))
        self.assertEqual(c.writes,[(p.TRACE_START,(9,2))])

    def test_progress_and_manual_disable(self):
        c=Fake();cap=self.begin(c)
        self.assertEqual(inspect_trial(c,cap),(1,"recording",None))
        c.trace=(9,9,2,8)
        self.assertEqual(inspect_trial(c,cap),(9,"disable",None))
        c.finish();n,stage,trial=inspect_trial(c,cap)
        self.assertEqual((n,stage),(9,"complete"));self.assertEqual(trial.metrics['settling_ms'],2)
        self.assertEqual(len(c.writes),2)  # progress never sends a command

    def test_progress_rejects_interrupted_or_stuck(self):
        c=Fake();cap=self.begin(c);c.tick=2000
        with self.assertRaisesRegex(ValueError,'expected window'):inspect_trial(c,cap)
        c.trace=(9,9,2,12)
        with self.assertRaises(ValueError):inspect_trial(c,cap)
        c.trace=(1,10,2,9)
        with self.assertRaises(ValueError):inspect_trial(c,cap)

    def test_apply_candidate_requires_disabled_matching_machine(self):
        c=Fake();cap=self.begin(c);c.finish();baseline=finish_trial(c,cap)
        candidate=(2048,0,64,2,500)
        # SET_PID changes only normal gains in this simulated controller.
        original=c.write
        def write(op,*args):
            original(op,*args)
            if op==p.SET_PID:c.settings[p.GET_PID]=args
        c.write=write
        c.active=True
        with self.assertRaises(ValueError):apply_trial_gains(c,candidate,baseline)
        c.active=False
        c.settings[p.GET_LIMITS]=(-2000,2000,1)
        with self.assertRaises(ValueError):apply_trial_gains(c,candidate,baseline)
        c.settings[p.GET_LIMITS]=baseline.context[3]
        with self.assertRaises(ValueError):apply_trial_gains(c,(2048,0,64,2,501),baseline)
        self.assertEqual(len(c.writes),2)
        self.assertEqual(apply_trial_gains(c,candidate,baseline),candidate)
        self.assertEqual(c.writes[-1],(p.SET_PID,candidate))

    def test_apply_readback_mismatch_no_retry(self):
        c=Fake();cap=self.begin(c);c.finish();baseline=finish_trial(c,cap)
        with self.assertRaisesRegex(ValueError,'readback'):apply_trial_gains(c,(2048,0,64,2,500),baseline)
        self.assertEqual(len(c.writes),3)
