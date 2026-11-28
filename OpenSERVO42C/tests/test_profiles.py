import json
import unittest
from studio import protocol as p
from studio.profiles import GROUPS, capture, dumps, loads

BASE={'Protection':(4096,2048,100,800,100,100,2), 'PID':(4096,0,64,2,500),
      'Motion':(10000,30000,15000,60000,1), 'Drive':(16,1000,500,2048,300,25,0,600,0),
      'Travel':(-1000,1000,1)}

class Fake:
    def __init__(self): self.flags=0;self.live=0
    def run_status(self): return {'flags':self.flags}
    def read(self,op):
        if op==p.GET_LIVE_PID:return (self.live,)+BASE['PID']
        return next(BASE[name] for name,code,_ in GROUPS if code==op)

class ProfileTests(unittest.TestCase):
    def test_roundtrip(self):
        self.assertEqual(loads(dumps(BASE)),BASE)
        self.assertEqual(capture(Fake()),BASE)
    def test_bad_values(self):
        for group,idx,value in [('Protection',3,1001),('PID',0,1048577),('Motion',3,0),('Drive',7,500),('Travel',0,1001),('Drive',6,True),('PID',1,-1)]:
            bad=dict(BASE);v=list(bad[group]);v[idx]=value;bad[group]=v
            with self.assertRaises(ValueError):dumps(bad)
    def test_wrong_version_missing_group(self):
        data=json.loads(dumps(BASE));data['version']=True
        with self.assertRaises(ValueError):loads(json.dumps(data))
        data['version']=1;del data['settings']['Travel']
        with self.assertRaises(ValueError):loads(json.dumps(data))
    def test_active_capture_rejected(self):
        for flags,live in [(1,0),(2,0),(16,0),(0,1)]:
            c=Fake();c.flags=flags;c.live=live
            with self.assertRaises(ValueError):capture(c)
