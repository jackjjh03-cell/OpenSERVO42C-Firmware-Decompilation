"""Optional machine-instruction checks against the recovered original data and IRQ.
Uses the cumulative project's limited Thumb interpreter, never real hardware.
"""
import sys, pathlib, struct, json, random, hashlib
ROOT=pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT.parent/'tests'))
from differential_m3 import M3Thumb,load_ins
class LayoutVM(M3Thumb):
 def __init__(self,*a,callbacks=None):super().__init__(*a);self.callbacks=callbacks or {}
 def step(self,p):
  if p in self.callbacks:
   name,n,value=self.callbacks[p];self.calls.append((name,tuple(self.r[:n])));self.r[0]=value;return self.r[14]&~1
  return super().step(p)
 def execute(self,p):
  self.r[13]=0x200015f8
  for _ in range(100000):
   if p==0xfffffffc:return
   self.r[15]=p;p=self.step(p)
  raise AssertionError('instruction limit')

def check(elf,flash,syms,vector_words):
 meta=json.loads((ROOT/'tests/layout_expected.json').read_text())
 # Validate every nonzero vector mapping, not only a sample.
 expected=[syms['__stack_top']]+[(syms[x]|1) if x else 0 for x in meta['vectors'][1:]]
 assert list(vector_words)==expected
 for a in meta['assets']:
  addr=syms[a['name']];assert hashlib.sha256(flash[addr:addr+a['size']]).hexdigest()==a['sha256'],a['name']
 ins=load_ins(elf,0xc0,len(flash));expected=bytearray((ROOT/'tests/original_initialized_ram.bin').read_bytes()+bytes(meta['cleared_bytes']))
 for r in meta['callbacks']:struct.pack_into('<I',expected,r['offset']+12,syms[r['symbol']]|1)
 rng=random.Random(0x42c13)
 for k in range(8):
  init=bytearray(rng.getrandbits(8192*8).to_bytes(8192,'little'));vm=LayoutVM(flash,ins,init,{})
  vm.execute(syms['initialize_factory_data']&~1)
  assert vm.ram[:len(expected)]==expected,('startup bytes',k,[(i,x,y) for i,(x,y) in enumerate(zip(vm.ram,expected)) if x!=y][:10]);assert vm.r[13]==0x200015f8
 print(f'8 compiled startup-data executions matched all {len(expected)} bytes after callback relocation; all 48 vector words and {len(meta["assets"])} asset hashes verified.')
 # Compare actual original auxiliary IRQ instructions against independently linked C.
 baseline=ROOT.parent/'build/firmware.elf';original=(ROOT.parent/'reference/factory.bin').read_bytes()
 if not baseline.exists():raise SystemExit('Run python tools/build.py in the cumulative project before optional comparison.')
 oi=load_ins(baseline,0x28c8,0x2a08)
 def put(r,o,v,n):r[o:o+n]=(v&((1<<(8*n))-1)).to_bytes(n,'little')
 for k in range(1024):
  r=bytearray(rng.getrandbits(8192*8).to_bytes(8192,'little'));flag=k%4!=0
  put(r,0x13,(k//4)%3,1);put(r,0x735,(k//12)%3,1);put(r,0x737,(k//36)%2,1);put(r,0x608,(k//72)%2,1)
  a=rng.choice([rng.randrange(-100000,100001),0,0x7fffffffffffffff,-0x8000000000000000]);t=rng.choice([a,a+1,a-1,a+65536,a-65536,rng.randrange(-100000,100001)]);put(r,0x680,a,8);put(r,0x610,t,8)
  put(r,0x18,[0,23,24,25,48,49,50,0xffffffff][k%8],4);put(r,0x618,[0,1,16,65535][k%4],2);put(r,0x5e0,[0,4095,4096,65535][(k//4)%4],2);put(r,0x62c,0x20000358 if k%2 else 0x20000560,4)
  calls={0x08d8:('test_flag',2,int(flag)),0x08c0:('clear_flag',2,0),0x2334:('uart_reply',0,0)}
  nc={syms['timer_test_flag']&~1:('test_flag',2,int(flag)),syms['timer_clear_flag']&~1:('clear_flag',2,0),syms['uart_start_reply']&~1:('uart_reply',0,0)}
  x=LayoutVM(original,oi,r,{},callbacks=calls);y=LayoutVM(flash,ins,r,{},callbacks=nc)
  x.execute(0x28c8);y.execute(syms['auxiliary_timer_irq']&~1)
  assert x.ram[:0xa54]==y.ram[:0xa54] and x.calls==y.calls and x.r[13]==y.r[13],(k,a,t,x.calls,y.calls,[(i,v,w) for i,(v,w) in enumerate(zip(x.ram[:0xa54],y.ram[:0xa54])) if v!=w])
 print('1,024 auxiliary timer IRQ cases matched original machine code (RAM effects and modeled call sequence; no concurrent interrupt or hardware timing simulation).')
