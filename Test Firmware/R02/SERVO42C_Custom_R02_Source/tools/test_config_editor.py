import config_editor as c
class Mock:
 def __init__(self):self.values={0:2,24:0,25:0,31:0,21:65535};self.writes=[];self.reply=b''
 def reset_input_buffer(self):self.reply=b''
 def write(self,frame):
  assert (sum(frame[:-1])&255)==frame[-1]
  self.writes.append(frame);a,op=frame[:2]
  if op==0x8d:v=self.values.get(frame[2],0);n=4
  elif op==0x3a:v=2;n=1
  elif op==0xa1:self.values[21]=int.from_bytes(frame[2:4],'big');v=1;n=1
  else:raise AssertionError('unexpected opcode')
  r=bytes([a])+v.to_bytes(n,'big');self.reply=r+bytes([sum(r)&255])
 def flush(self):pass
 def read(self,n):return self.reply[:n]
m=Mock();d=c.Controller(m,0xe0);assert d.query(21)==65535
assert d.set_tuning(21,1638)==1638;assert any(f[1]==0xa1 for f in m.writes)
for idx in (24,25,31):
 m.values[idx]=1;old=len([f for f in m.writes if f[1]==0xa1])
 try:d.set_tuning(21,100)
 except RuntimeError:pass
 else:raise AssertionError('unsafe state accepted')
 assert len([f for f in m.writes if f[1]==0xa1])==old;m.values[idx]=0
try:c.parse_reply(b'\xe0\x01\x00',0xe0,1)
except IOError:pass
else:raise AssertionError('bad checksum accepted')
print('PASS: host editor mock serial query, confirmed setter/readback, three drive/motion guards and checksum failure.')
