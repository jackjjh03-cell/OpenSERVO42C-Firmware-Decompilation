"""Factory-versus-standalone UART control-flow comparisons.
Requires private factory binary/ELF. No physical peripheral timing is modeled.
Calls to command side effects are intercepted and compared at their ABI.
"""
from integration import VM, put, ROOT
from thumb_model import load_ins
import random

OPS=[0x30,0x33,0x36,0x39,0x3a,0x3d,0x3e,0x3f,*range(0x80,0x8d),*range(0x90,0x95),*range(0xa1,0xa6),0xf3,0xf6,0xf7,0xfd,0xff]
# Factory callee address and observed argument count. New callbacks use
# relocated symbol addresses; no assumptions about code size or placement.
CALLEES={
 'factory_start_reply':(0x2334,0),'uart_reset_rx':(0x5ce8,0),
 'factory_restore':(0x3938,0),'factory_calibrate':(0x2e38,1),
 'factory_format_uint':(0x3052,2),'factory_timer_reload':(0x0908,2),
 'factory_timer_start':(0x0924,1),'factory_timer_stop':(0x093c,1),
 'factory_stop':(0x3a18,0),'factory_commit':(0x470c,0),
 'factory_refresh_mode':(0x2090,0),
 'menu_control_mode':(0x2d34,1),'menu_acceleration':(0x3fcc,1),
 'menu_autosdd':(0x404c,1),'menu_uart_baud':(0x4084,1),
 'menu_current':(0x41c8,1),'menu_direction':(0x420c,1),
 'menu_enable':(0x4244,1),'menu_interpolation':(0x4288,1),
 'menu_microsteps':(0x42c0,1),'menu_motor_type':(0x4360,1),
 'menu_uart_address':(0x439c,1),'menu_origin_direction':(0x43d0,1),
 'menu_origin_speed':(0x4400,1),'menu_origin_mode':(0x442c,1),
 'menu_protection':(0x4464,1)}

def check_protocol(elf,flash,syms):
 oldelf=ROOT.parent/'build/firmware.elf'; oldbin=ROOT.parent/'reference/factory.bin'
 if not oldelf.exists() or not oldbin.exists():
  print('SKIP: private factory fixtures absent; protocol comparison requires private verification archive.');return
 oi=load_ins(oldelf,0xc0,0x7a00);ni=load_ins(elf,0xc0,syms['__flash_code_end']);original=oldbin.read_bytes();rng=random.Random(0x42c20)
 addr=lambda n:syms[n]&~1
 count=0
 def compare(op,arg,mode,state,matching=True):
  nonlocal count
  r=bytearray(rng.getrandbits(0xa54*8).to_bytes(0xa54,'little')+bytes(8192-0xa54))
  address=0xe0+arg%10
  put(r,0x740,address,2);put(r,0x735,mode,1);put(r,0x6ab,address if matching else (address^1),1);put(r,0x6ac,op,1);put(r,0x6ad,arg,1);put(r,0x13,state,1);put(r,0x5f6,state,1);put(r,0x5f8,65535 if arg%4==0 else 100,2)
  put(r,0x72c,(256 if not arg else arg),2);put(r,0x72e,arg if state else 16,2)
  events=[[],[]];cal=1 if arg%2 else 2
  def cb(name,n,ev):
   def call(v):
    ev.append((name,tuple(v.r[:n])))
    if name=='factory_calibrate':v.r[0]=cal
   return call
  a={p:(name,cb(name,n,events[0])) for name,(p,n) in CALLEES.items()};b={addr(name):(name,cb(name,n,events[1])) for name,(p,n) in CALLEES.items()}
  mmio={0x4000580c:rng.getrandbits(32)}
  x=VM(original,oi,r,mmio,legacy=True,callbacks=a);y=VM(flash,ni,r,mmio,callbacks=b)
  x.execute(0x70dc);y.execute(addr('uart_dispatch'))
  assert x.ram[:0xa54]==y.ram[:0xa54] and x.mmio==y.mmio and events[0]==events[1],('dispatch',hex(op),arg,mode,state,matching,[(hex(i),u,v) for i,(u,v) in enumerate(zip(x.ram[:0xa54],y.ram[:0xa54])) if u!=v][:12],events,x.mmio,y.mmio)
  count+=1
 for op in OPS:
  for arg in range(256):compare(op,arg,arg%3,(arg//3)%3)
 for op in (0x30,0x80,0x84,0x91,0x94,0xf3,0xf6,0xf7,0xfd,0xff):
  for mode in range(3):
   for state in range(3):
    for arg in (0,1,2,3,4,8,16,32,64,127,128,200,202,255):compare(op,arg,mode,state)
 for op in range(256):
  if op not in OPS and op!=0x8D:compare(op,0,2,1)
  compare(op,0,2,1,False)
 print(f'PASS: {count} relocated UART dispatch cases against factory; all 36 opcodes, all byte arguments, unknown opcodes, wrong addresses, selected mode/state combinations. Callee arguments and workspace/MMIO effects compared; external effects mocked.',flush=True)
 # Byte-stream parser, with dispatch intercepted to avoid physical actions.
 # Keep ready set until a modeled reset, just as a real outstanding packet.
 nstreams=nbytes=0
 def stream(data,address,state):
  nonlocal nstreams,nbytes
  r=bytearray(8192);put(r,0x740,address,2);put(r,0x13,state,1)
  x=VM(original,oi,r,{},legacy=True,callbacks={0x70dc:('dispatch',lambda v:None)})
  y=VM(flash,ni,r,{},callbacks={addr('factory_dispatch'):('dispatch',lambda v:None),addr('uart_dispatch'):('dispatch',lambda v:None)})
  for byte in data:
   for v,entry in ((x,0x2a38),(y,addr('uart0_irq'))):
    v.mmio.update({0x40000010:1,0x40000000:byte,0x40000014:3});v.execute(entry)
   assert x.ram[:0xa54]==y.ram[:0xa54] and x.mmio==y.mmio and x.calls==y.calls,('stream',address,state,data,byte,x.ram[0x6a8:0x6bc].hex(),y.ram[0x6a8:0x6bc].hex(),x.calls,y.calls)
   nbytes+=1
  nstreams+=1
 for op in range(256):
  a=0xe0+op%10
  for payload in ([],[a],[1,2],[0,a,0xff,0,1]):
   frame=[a,op]+payload;frame.append(sum(frame)&255)
   stream(frame,a,op%3);stream(frame[:-1]+[frame[-1]^1]+[a,0x3a,(a+0x3a)&255],a,2)
 for _ in range(256):
  a=0xe0+rng.randrange(10);stream([rng.choice([a,rng.randrange(256)]) for _ in range(16)],a,2)
 print(f'PASS: {nstreams} UART streams / {nbytes} RX interrupts; malformed checksums, embedded addresses, unknown opcodes, resynchronization and busy receiver state. Factory dispatch mocked at the same ABI boundary.',flush=True)
 # Exercise the actual relocated menu callbacks, including their settings copy;
 # intercept the physical commit and mode-refresh operations on both sides.
 menu_limits={'menu_control_mode':2,'menu_acceleration':7,'menu_autosdd':1,'menu_uart_baud':6,'menu_current':12,'menu_direction':1,'menu_enable':2,'menu_interpolation':1,'menu_microsteps':9,'menu_motor_type':1,'menu_uart_address':9,'menu_origin_direction':1,'menu_origin_speed':4,'menu_origin_mode':2,'menu_protection':1}
 nmenu=0
 for name,maxarg in menu_limits.items():
  for arg in [*range(maxarg+1),255]:
   for k in range(16):
    r=bytearray(rng.getrandbits(0xa54*8).to_bytes(0xa54,'little')+bytes(8192-0xa54))
    put(r,0x72c,1<<(k%9),2);put(r,0x72e,k+1,2);put(r,0x730,k%2,1);put(r,0x734,k%3,1);put(r,0x735,k%3,1)
    for o in (0x736,0x737,0x738,0x5fc):put(r,o,k%2,1)
    put(r,0x73c,[0,9600,19200,25000,38400,57600,115200,12345][k%8],4);put(r,0x740,0xe0+k%10,2);put(r,0x744,k%8,2);put(r,0x5e4,(k%13)*200,2);put(r,0x5e8,25 if k%2 else 50,2);put(r,0x5f6,k%3,1);put(r,0x5fa,k%5,2)
    def nop(v):pass
    x=VM(original,oi,r,{},legacy=True,callbacks={0x470c:('commit',nop),0x2090:('refresh',nop)})
    y=VM(flash,ni,r,{},callbacks={addr('factory_commit'):('commit',nop),addr('factory_refresh_mode'):('refresh',nop)})
    x.r[0]=arg;y.r[0]=arg;x.execute(CALLEES[name][0]);y.execute(addr(name))
    # Only query return values are consumed by the UI. Set callbacks' return
    # registers are unused; compare their actual state/call effects instead.
    assert (arg!=255 or x.r[0]==y.r[0]) and x.ram[:0xa54]==y.ram[:0xa54] and x.mmio==y.mmio and x.calls==y.calls,('menu',name,arg,k,x.r[0],y.r[0],[(hex(i),u,v) for i,(u,v) in enumerate(zip(x.ram[:0xa54],y.ram[:0xa54])) if u!=v][:12],x.calls,y.calls)
    nmenu+=1
 print(f'PASS: {nmenu} relocated menu callbacks; every supported option and query, settings copy and commit/refresh call sequence compared with factory instructions.',flush=True)
 # STOP updates the complete motion state; intercept only the timer operation.
 for k in range(128):
  r=bytearray(rng.getrandbits(0xa54*8).to_bytes(0xa54,'little')+bytes(8192-0xa54));mmio={0x4000580c:rng.getrandbits(32)}
  events=[[],[]]
  def stop0(v):events[0].append(v.r[0])
  def stop1(v):events[1].append(v.r[0])
  x=VM(original,oi,r,mmio,legacy=True,callbacks={0x093c:('stop_timer',stop0)})
  y=VM(flash,ni,r,mmio,callbacks={addr('factory_timer_stop'):('stop_timer',stop1)})
  x.execute(0x3a18);y.execute(addr('factory_stop'))
  assert x.ram[:0xa54]==y.ram[:0xa54] and x.mmio==y.mmio and events[0]==events[1]==[2],('stop',k)
 print('PASS: 128 relocated STOP motion-state comparisons; physical timer stop mocked.',flush=True)
