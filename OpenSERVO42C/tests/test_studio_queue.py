"""GUI queue admission without a display or a physical serial port."""
import unittest
from concurrent.futures import Future
from threading import Event
from studio.app import Studio

class Message:
    def set(self, value): self.value=value

class Worker:
    def submit(self, fn):
        self.fn=fn
        return Future()

class QueueTests(unittest.TestCase):
    def app(self):
        a=Studio.__new__(Studio)
        a.connected=True;a.link_uncertain=False;a.recovery=None;a.closing=False;a.cal_active=False;a.lut_active=False
        a.cancel_trial=Event();a.future=None;a.pending=None;a.quiet=True
        a.worker=Worker();a.message=Message();a.client=None
        return a

    def test_calibration_excludes_user_commands(self):
        for flag in ('cal_active','lut_active'):
            a=self.app();setattr(a,flag,True)
            self.assertFalse(a.submit(lambda c:None));self.assertIsNone(a.future)
            self.assertTrue(a.submit(lambda c:None,quiet=True,internal=True))
            self.assertFalse(a.submit(lambda c:None));self.assertIsNone(a.pending)
            self.assertTrue(a.submit(lambda c:None,urgent=True))
            self.assertTrue(a.pending[2]);self.assertTrue(a.cancel_trial.is_set())

    def test_urgent_replaces_pending_normal(self):
        a=self.app();a.submit(lambda c:None,quiet=True)
        move=lambda c:None;stop=lambda c:None
        self.assertTrue(a.submit(move));self.assertTrue(a.submit(stop,urgent=True))
        self.assertIs(a.pending[0],stop);self.assertFalse(a.submit(move))
        self.assertIs(a.pending[0],stop)

    def test_closing_admission(self):
        a=self.app();a.closing=True
        self.assertFalse(a.submit(lambda c:None));self.assertTrue(a.submit(lambda c:None,internal=True))

    def test_uncertain_or_recovery_blocks_ordinary_commands(self):
        for name,value in (('link_uncertain',True),('recovery','LUT upload')):
            a=self.app();setattr(a,name,value)
            self.assertFalse(a.submit(lambda c:None));self.assertIsNone(a.future)
            self.assertTrue(a.submit(lambda c:None,urgent=True))
