"""Run independently linked Thumb code; stub only explicit startup boundaries.
UART tests execute the actual parser/dispatcher/transmit code and compare to
factory replies when the factory fixture is present. No motor hardware model.
"""
import json,struct,random,pathlib
from thumb_model import M3Thumb,load_ins
ROOT=pathlib.Path(__file__).resolve().parents[1]
class Checkpoint(Exception):pass
class VM(M3Thumb):
 def __init__(self,*a,callbacks=None,legacy=False):
  super().__init__(*a);self.callbacks=callbacks or {};self.legacy_addresses=legacy;self.steps=0;self.low_sp=0x200015f8
 def read(self,a,n):
  if 0xe0000000<=a<0xe0100000 and n==4:return self.mmio.get(a,0)
  return super().read(a,n)
 def write(self,a,n,v):
  if 0xe0000000<=a<0xe0100000 and n==4:self.mmio[a]=v&0xffffffff;return
  return super().write(a,n,v)
 def step(self,p):
  if p in self.callbacks:
   name,fn=self.callbacks[p];self.calls.append(name);fn(self);return self.r[14]&~1
  if self.legacy_addresses and p==0x15ec:
   a=self.r[14]&~1;return (a+1+2*self.read(a+1+min(self.r[3],self.read(a,1)),1))&~1
  if self.legacy_addresses and p in (0x1b0,0x13c):
   if p==0x1b0:
    a=self.r[0]|self.r[1]<<32;b=self.r[2]|self.r[3]<<32;v=a*b;self.r[0]=v&0xffffffff;self.r[1]=(v>>32)&0xffffffff
   else:
    n,d=(x if x<0x80000000 else x-0x100000000 for x in self.r[:2]);assert d;q=(abs(n)//abs(d))*(-1 if (n<0)!=(d<0) else 1);self.r[0]=q&0xffffffff;self.r[1]=(n-q*d)&0xffffffff
   return self.r[14]&~1
  if p in self.ins and self.ins[p][1]=='cmn':
   size,_,arg=self.ins[p];a,b=arg.split(', ');self.add(self.val(a,p),self.val(b,p));return p+size
  return super().step(p)
 def execute(self,p,limit=100000):
  self.r[13]=0x200015f8;self.r[14]=0xfffffffd
  for i in range(limit):
   if p==0xfffffffc:return
   self.r[15]=p;self.low_sp=min(self.low_sp,self.r[13]);assert self.r[13]>=0x20000a54,'stack overlaps legacy workspace';self.steps+=1;p=self.step(p)
  raise AssertionError(('instruction limit',hex(p)))
def put(r,o,v,n):r[o:o+n]=(v&((1<<(8*n))-1)).to_bytes(n,'little')
def check_integration(elf,flash,syms,words,edit=False):
 ins=load_ins(elf,0xc0,syms['__flash_code_end']);rng=random.Random(0x42c14)
 # Full reset through real startup RAM initialization and real application_main;
 # all physical setup/diagnostic functions are explicit checkpoint mocks.
 expected=bytearray((ROOT/'tests/original_initialized_ram.bin').read_bytes()+bytes(0x42c));meta=json.loads((ROOT/'tests/layout_expected.json').read_text())
 for row in meta['callbacks']:struct.pack_into('<I',expected,row['offset']+12,syms[row['symbol']]|1)
 if edit:
  i=expected.index(b'UartBaud\0');expected[i:i+12]=b'Serial\0'+bytes(5)
 def addr(n):return syms[n]&~1
 for valid in [False,True]:
  for k in range(8):
   ram=bytearray(rng.getrandbits(8192*8).to_bytes(8192,'little'))
   def nothing(v):pass
   def identity(v):v.r[0]=int(valid)
   def board(v):
    e=bytearray(expected);put(e,12,1,4);assert v.ram[:len(e)]==e,'reset startup RAM differs'
    if edit:
     assert v.read(syms['edit_counter'],4)==0x12345678;assert v.read(syms['edit_zero'],4)==0x12345678
   def stop(v):raise Checkpoint
   funcs={'factory_system_init':nothing,'factory_board_init':board,'factory_identity_check':identity,'factory_config_pins':nothing,'factory_startup_diagnostics':nothing,'fault_indicator_toggle':stop,'factory_foreground_ui':stop}
   v=VM(flash,ins,ram,{},callbacks={addr(n):(n,f) for n,f in funcs.items()})
   try:v.execute(words[1]&~1)
   except Checkpoint:pass
   else:raise AssertionError('reset unexpectedly returned')
   want=['factory_system_init','factory_system_init','factory_board_init','factory_identity_check','factory_config_pins']+(['factory_startup_diagnostics','factory_foreground_ui'] if valid else ['fault_indicator_toggle'])
   assert v.calls==want,(v.calls,want);assert v.read(0xe000ed08,4)==0
   assert v.read(0x2000000c,4)==(0 if valid else 1)
 print('PASS: 16 reset-to-application checkpoint runs; initialized RAM, identity success/failure branches and startup flag verified. Physical initialization and diagnostics mocked.')
 if edit:return
 # UART: entire parser + dispatcher + response start + interrupt TX end, no
 # callee mocks in relocated code. Use expected packet values as an independent oracle.
 original=ROOT.parent/'reference/factory.bin'; oldelf=ROOT.parent/'build/firmware.elf'
 oldins=load_ins(oldelf,0xc0,0x7a00) if oldelf.exists() and original.exists() else None
 oldflash=original.read_bytes() if oldins else None
 def frame(v,entry,command,address):
  tx=[]
  for byte in [address,command,(address+command)&255]:
   v.mmio.update({0x40000010:1,0x40000000:byte});v.execute(entry)
  assert v.ram[0x6aa]>0,('no reply',command,address,v.ram[0x6a8:0x6bc].hex())
  tx.append(v.mmio[0x40000000]&255)
  for i in range(8):
   if not v.ram[0x6aa]:break
   v.mmio[0x40000010]=2;v.execute(entry);tx.append(v.mmio[0x40000000]&255)
  assert v.ram[0x6aa]==0
  v.mmio[0x40000010]=2;v.execute(entry)
  assert v.ram[0x6a8:0x6aa]==b'\0\0' and v.ram[0x6ab:0x6b4]==bytes(9)
  return bytes(tx)
 count=0
 for k in range(64):
  for op in (0x30,0x33,0x36,0x39,0x3a,0x3e):
   address=0xe0+k%10;mode=k%3;r=bytearray(8192);put(r,0x740,address,2);put(r,0x735,mode,1)
   for o in (0x638,0x63c,0x680,0x664,0x690,0x668,0x66c):put(r,o,rng.getrandbits(32),4)
   r[0x5e2]=k%2;r[0x649]=(k//2)%2
   def w(o,n=4):return int.from_bytes(r[o:o+n],'little')
   if op==0x30:payload=(((w(0x63c)&65535)<<32)|w(0x638)).to_bytes(6,'big') if mode else b'\0'
   elif op==0x33:payload=((w(0x680)-w(0x664))&0xffffffff).to_bytes(4,'big')
   elif op==0x36:payload=((w(0x690)-w(0x668))&0xffffffff).to_bytes(4,'big')
   elif op==0x39:payload=w(0x66c,2).to_bytes(2,'big')
   elif op==0x3a:payload=bytes([1 if r[0x5e2] else 2])
   else:payload=bytes([1 if r[0x649] else 2])
   want=bytes([address])+payload;want+=bytes([sum(want)&255]);mmio={0x40020d00:0x4000,0x40020c38:0,0x40000004:1,0x40000014:3}
   v=VM(flash,ins,r,mmio);got=frame(v,addr('uart0_irq'),op,address);assert got==want,(op,k,got.hex(),want.hex())
   if oldins:
    old=VM(oldflash,oldins,r,mmio,legacy=True);oldgot=frame(old,0x2a38,op,address);assert oldgot==got,(op,k,oldgot.hex(),got.hex());assert old.ram[:0xa54]==v.ram[:0xa54] and old.mmio==v.mmio,(op,k,'side effects')
   count+=1
 print(f'PASS: {count} full read-only UART exchanges, byte-level packet oracle'+(' and factory instruction comparisons (RAM/MMIO effects)' if oldins else '')+'.')
 # Load settings from blank, sparse, full and invalid records. This is read-only.
 for k in range(128):
  page=bytearray(b'\xff'*512);slot=k%8
  if k%4:
   page[slot*64:(slot+1)*64]=rng.getrandbits(512).to_bytes(64,'little')
   put(page,slot*64,16,2);put(page,slot*64+54,int(k%4!=1),2)
   if k%4==3 and slot:put(page,0,32,2);put(page,54,1,2)
  f=bytearray(flash);f[0x7c00:0x7e00]=page;r=bytearray(rng.getrandbits(8192*8).to_bytes(8192,'little'))
  v=VM(bytes(f),ins,r,{});v.execute(addr('load_or_initialize_settings'))
  assert v.read(0x20000004,4)==(0x7c00+slot*64 if k%4 else 0x7c00)
  if k%4 in (0,1):assert v.read(0x2000072c,2)==16 and v.read(0x20000740,2)==0xe0 and v.read(0x20000650,2)==1
  else:assert v.read(0x2000072c,2)==16 and v.read(0x20000740,2)==int.from_bytes(page[slot*64+20:slot*64+22],'little')
  if oldins:
   f=bytearray(oldflash);f[0x7c00:0x7e00]=page;old=VM(bytes(f),oldins,r,{},legacy=True);old.execute(0x5368);assert old.ram[:0xa54]==v.ram[:0xa54],('settings',k)
 print('PASS: 128 settings-load scenarios, including blank and invalid records'+(' compared with factory instructions' if oldins else '')+'.')
 # Safety-critical offset check: execute the arithmetic and branches in actual
 # independently linked instructions. ADC/timer/display functions are modeled.
 oldcalls={'adc_start':0x7b0,'factory_delay_ms':0x2de8,'adc_test_flags':0x738,'adc_read_result':0x74c,'adc_clear_flags':0x718,'adc_stop':0x7bc,'display_three_lines':0x4884,'timer_config_20':0xb14,'timer_clear_all':0x8b0,'timer_clear_flag':0x8c0,'adc_interrupt_mask':0x7c8}
 # Actual ADC clear helper address is taken from the factory index.
 cases=0
 for a in [1860,1861,1862,2048,2233,2234,2235]:
  for b in [1860,1861,2048,2234,2235]:
   for spread in [0,100]:
    samples=[(a,b)]*18;samples[0]=(a-spread,b+spread);samples[17]=(a+spread,b-spread)
    r=bytearray(8192);expect_pass=1861<=a<=2234 and 1861<=b<=2234
    def offset_run(f,ii,addresses,legacy):
     events=[];reads=[0];polls=[0]
     def fun(name):
      def act(v):
       if name=='adc_read_result':
        channel=v.r[0];assert channel==(2 if reads[0]%2==0 else 1);value=samples[reads[0]//2][reads[0]%2];reads[0]+=1;v.r[0]=value;events.append((name,channel,value))
       elif name=='adc_test_flags':
        assert v.r[0]==32;polls[0]+=1;v.r[0]=int(polls[0]%2==0);events.append((name,v.r[0]))
       elif name=='display_three_lines':
        def txt(p):
         t=bytearray()
         while v.read(p+len(t),1):t.append(v.read(p+len(t),1))
         return bytes(t)
        events.append((name,tuple(txt(p) for p in v.r[:3])));raise Checkpoint
       elif name=='timer_config_20':events.append((name,v.r[0],v.read(v.r[1],4)));v.r[0]=0
       else:
        n={'adc_start':0,'adc_stop':0,'factory_delay_ms':1,'adc_clear_flags':1,'timer_clear_all':1,'timer_clear_flag':2,'adc_interrupt_mask':2}[name];events.append((name,tuple(v.r[:n])))
      return act
     v=VM(f,ii,r,{},legacy=legacy,callbacks={p:(name,fun(name)) for name,p in addresses.items()});v.r[1]=0
     try:v.execute(0x2464 if legacy else addr('check_current_offsets'));passed=True
     except Checkpoint:passed=False
     assert reads[0]==36 and polls[0]==36
     assert passed==expect_pass and v.read(0x200006e4,4)==a and v.read(0x200006e8,4)==b,(a,b,passed,expect_pass)
     if not passed:assert events[-1]==('display_three_lines',(b'Offset',b' Current',b'Error!'))
     return events
    ev=offset_run(flash,ins,{name:addr(name) for name in oldcalls},False)
    if oldins:assert ev==offset_run(oldflash,oldins,oldcalls,True),(a,b,spread)
    cases+=1
 print(f'PASS: {cases} startup current-offset scenarios (inclusive limits, outliers, polling, and error-vs-success control flow)'+(' matched factory instructions' if oldins else '')+'. ADC values and hardware calls modeled; not electrical validation.')
 # Motion interrupt: actual relocated arithmetic, encoder table access, and
 # MMIO sequences. Only the physical encoder read is replaced by a sample.
 if oldins:
  f=bytearray(flash);f[0x7a00:]=oldflash[0x7a00:];f=bytes(f)
  for k in range(96):
   ram=bytearray(rng.getrandbits(8192*8).to_bytes(8192,'little'))
   mmio={a:rng.getrandbits(32) for base in (0x40002000,0x40020000,0x40021000,0x40002400,0x40002480,0x40000c00,0x40000d00,0x40020d00,0x40020e00,0x40003000,0x40005800,0x40021800) for a in range(base,base+256,4)}
   ram[0x735]=k%3;ram[0x13]=(k//3)%4;ram[0x738]=(k//12)%2;ram[0x737]=(k//24)%2;ram[0xb]=1
   for o,values,n in [(0x72c,[16,32,128,166,167,256],2),(0x732,[0,3276,65535],2),(0x5e8,[25,50],2),(0x654,[8192,4096,65536,1],4),(0x660,[0,1,2000,65535],4),(0x674,[0,998,999,1000,1001],4),(0x6dc,[0,4000,4001,8000,-8000],4)]:put(ram,o,values[k%len(values)],n)
   for o,v,n in [(0x5e4,k*8,2),(0x658,3000,4),(0x65c,-3000,4),(0x5ea,1638,2),(0x5ec,1,2),(0x5ee,1638,2)]:put(ram,o,v,n)
   for o in (0x680,0x688,0x690,0x698,0x6a0):put(ram,o,rng.randrange(-100000,100001),8)
   encoder=rng.randrange(16384)
   if k%7==0:
    put(ram,0x674,999,4);put(ram,0x6dc,5000,4);ram[0x737]=1;ram[0x735]=1
    angle=int.from_bytes(oldflash[0x8000+2*encoder:0x8002+2*encoder],'little');put(ram,0x690,angle,8);put(ram,0x698,angle,8)
   def read_encoder(v):v.r[0]=encoder
   # Stub interrupt configuration (barriers are not modeled), compare other MMIO.
   def no_op(v):pass
   a=VM(oldflash,oldins,ram,mmio,legacy=True,callbacks={0x51dc:('encoder',read_encoder),0xfec:('irq_config',no_op)})
   b=VM(f,ins,ram,mmio,callbacks={addr('factory_read_encoder'):('encoder',read_encoder),addr('factory_irq_config'):('irq_config',no_op)})
   a.execute(0x6d14);b.execute(addr('motion_timer_irq'))
   assert a.ram[:0xa54]==b.ram[:0xa54] and a.mmio==b.mmio and a.calls==b.calls,('motion',k,[(hex(i),x,y) for i,(x,y) in enumerate(zip(a.ram[:0xa54],b.ram[:0xa54])) if x!=y][:12],a.calls,b.calls)
  print('PASS: 96 relocated motion-timer IRQ scenarios including stall threshold and protection; factory RAM and MMIO compared. Encoder and NVIC configuration modeled.')
  for k in range(96):
   ram=bytearray(rng.getrandbits(8192*8).to_bytes(8192,'little'))
   mmio={a:rng.getrandbits(32) for base in (0x40021000,0x40002400,0x40002480,0x40000c00,0x40000d00,0x40021800) for a in range(base,base+256,4)}
   put(ram,0x8e0,[0,744,745,2048,4095][k%5],4);ram[0xb]=k%2;ram[0x6c0]=(k//2)%2
   put(ram,0x5e8,[25,50][k%2],2);put(ram,0x746,[0,600,1080,1200][k%4],2);put(ram,0xc,k%2,4)
   mmio[0x40021014]=0x50000 if k%2 else 0;mmio[0x40021814]=[1,2,3][k%3];mmio[0x40021808]=rng.randrange(2000);mmio[0x4002180c]=rng.getrandbits(32)
   a=VM(oldflash,oldins,ram,mmio,legacy=True);b=VM(f,ins,ram,mmio)
   a.execute(0x464);b.execute(addr('current_adc_irq'))
   assert a.ram[:0xa54]==b.ram[:0xa54] and a.mmio==b.mmio,('current ADC',k,[(hex(i),x,y) for i,(x,y) in enumerate(zip(a.ram[:0xa54],b.ram[:0xa54])) if x!=y][:12])
  print('PASS: 96 relocated current ADC IRQ scenarios including PWM outputs and low-input reset; RAM and modeled MMIO matched factory instructions.')
