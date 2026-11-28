import tempfile
import unittest
from pathlib import Path
from studio import protocol as p, preferences as pref
from studio.client import DeviceRejected, LinkError
from studio.session import probe


class Fake:
    def __init__(self, mode=None, version=(0,1,1,0)):
        self.mode=mode;self.version=version;self.reads=[]
    def read(self, op):
        self.reads.append(op)
        if op==p.HELLO:
            if self.mode in ('LUT upload','Calibration sweep'):raise DeviceRejected(op,2)
            return self.version
        if op==p.LUT_STATUS and self.mode=='LUT upload':return (16,1,1)
        if op==p.CAL_STATUS and self.mode=='Calibration sweep':return (1,2,1)
        raise DeviceRejected(op,2)
    def status(self):return {'state':0}
    def run_status(self):
        if self.mode=='Bench diagnostics only':raise DeviceRejected(p.GET_RUN,3)
        return {'flags':0}


class SessionTests(unittest.TestCase):
    def test_read_only_handshake(self):
        c=Fake();self.assertEqual(probe(c),(None,(0,1,1,0),({'state':0},{'flags':0})))
        with self.assertRaises(LinkError):probe(Fake(version=(2,0,0,0)))
    def test_recovery_modes(self):
        for mode in ('LUT upload','Calibration sweep','Bench diagnostics only'):
            c=Fake(mode);self.assertEqual(probe(c)[0],mode)
            c.mode=None;self.assertIsNone(probe(c)[0])
    def test_preferences(self):
        with tempfile.TemporaryDirectory() as folder:
            location=Path(folder)/'app'/'studio.json'
            self.assertEqual(pref.load(location),pref.DEFAULTS)
            values={'port':'COM12','graph_range':'10 s','graph_view':'Error','torque':500}
            pref.save(location,values)
            self.assertEqual(pref.load(location),{k:v for k,v in values.items() if k!='torque'})
            location.write_text('{',encoding='utf-8');self.assertEqual(pref.load(location),pref.DEFAULTS)
            self.assertEqual(pref.validate({'graph_view':'bad','port':'\n'}),pref.DEFAULTS)
    def test_graph_time_and_units(self):
        rows=[(0,1,2,3,400),(9,2,1,-3,500),(11,4,6,7,800)]
        self.assertEqual(pref.graph_series(rows,10,'Error')[0],[(9,-1),(11,2)])
        self.assertEqual(pref.graph_series(rows,10,'Load')[0],[(9,50),(11,80)])
        self.assertEqual(pref.graph_series(rows,10,'Speed')[0],[(9,-3),(11,7)])
        self.assertEqual(pref.graph_series(rows,10,'Position')[0],rows[1:])
