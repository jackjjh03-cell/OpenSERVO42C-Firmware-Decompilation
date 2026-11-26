"""Exercise relocated storage code with idealized NOR flash, not hardware."""
from integration import VM, put
from thumb_model import load_ins

class FlashVM(VM):
 def __init__(self,*args,**kwargs):
  super().__init__(*args,**kwargs);self.flash=bytearray(self.flash);self.operations=[];self.keys=[];self.busy=False;self.refuse_mode=False
 def read(self,a,n):
  if a==0x40020020 and self.busy:return super().read(a,n)|16
  return super().read(a,n)
 def write(self,a,n,v):
  if a==0x4002002c:self.keys.append(v)
  if a==0x40020020 and self.refuse_mode:return
  if 0<=a<0x10000:
   mode=self.mmio.get(0x40020020,0)&3
   assert self.keys[-2:]==[0x5a5a,0xa5a5]
   assert a>=0x7c00,('write outside settings/calibration',hex(a))
   if mode==2:
    assert n==1 and v==0;start=a&~511;self.flash[start:start+512]=b'\xff'*512;self.operations.append(('erase',start))
   elif mode==1:
    assert n==2 and a%2==0 and a<=0xfffe
    v&=65535;self.flash[a:a+2]=(self.read(a,2)&v).to_bytes(2,'little');self.operations.append(('program',a,v))
   else:raise AssertionError(('unexpected flash mode',mode))
   return
  super().write(a,n,v)

def check_persistence(elf,flash,syms):
 ins=load_ins(elf,0xc0,syms['__flash_code_end'])
 def addr(n):return syms[n]&~1
 def invoke(v,n,*args,limit=100000):
  v.r[:len(args)]=args;v.execute(addr(n),limit=limit);assert v.r[13]==0x200015f8;return v.r[0]
 for operation,base in [('flash_erase_page',0x7c00),('flash_program_halfword',0x7c40)]:
  for mode in ['normal','busy','refused','outside']:
   f=bytearray(flash);f[0x7c00:0x7e00]=bytes([0x5a])*512
   v=FlashVM(f,ins,bytearray(8192),{});v.busy=mode=='busy';v.refuse_mode=mode=='refused';a=0x10000 if mode=='outside' else base
   result=invoke(v,operation,a,0x1234)
   assert result=={'normal':0,'busy':9,'refused':9,'outside':4}[mode],(operation,mode,result)
   if mode=='normal':
    assert len(v.operations)==1
    assert v.read(base,2)==(65535 if operation=='flash_erase_page' else 0x1234&0x5a5a)
   else:assert not v.operations and v.flash==f
 print('PASS: 8 relocated flash primitive cases (success, busy timeout, rejected mode, out-of-range).')
 for k in range(64):
  f=bytearray(flash);page=bytearray(b'\xff'*512);slot=k%8
  for i in range(slot+1):
   put(page,i*64,16,2);put(page,i*64+54,1,2)
  scenario=(k//8)%4
  if scenario==1 and slot<7:put(page,(slot+1)*64,32,2)
  if scenario==2:put(page,slot*64+54,0,2)
  f[0x7c00:0x7e00]=page
  r=bytearray(8192);put(r,4,0x7c00+slot*64,4);put(r,0x5d9,k%5,1);put(r,0x648,k%3,1)
  for o,x in [(0x6f4,16),(0x710,1200),(0x712,600),(0x714,25),(0x708,0xe0+k%10)]:put(r,o,x,2)
  put(r,0x6fd,k%3,1)
  def nop(v):pass
  def mode(v):v.r[0]%=3
  callbacks={addr(n):(n,mode if n=='factory_apply_mode' else nop) for n in ['factory_pause_control','factory_resume_control','factory_apply_mode','factory_apply_direction','factory_apply_uart']}
  v=FlashVM(f,ins,r,{0x4000580c:k%2},callbacks=callbacks)
  invoke(v,'factory_commit',limit=200000)
  want=slot+1+(scenario==1)
  if scenario==2 or want>7:want=0
  base=0x7c00+64*want
  assert v.read(0x20000004,4)==base,(k,hex(v.read(0x20000004,4)),hex(base))
  assert v.operations[0]==(('erase',0x7c00) if want==0 else ('program',base,16)),(k,v.operations[:2])
  assert [x[1] for x in v.operations if x[0]=='program']==list(range(base,base+56,2))
  assert v.read(base+54,2)==1 and v.read(base,2)==16 and v.read(0x2000072c,2)==16
  assert v.flash[:0x7c00]==f[:0x7c00] and v.flash[0x7e00:]==f[0x7e00:]
  v2=FlashVM(v.flash,ins,bytearray(8192),{});invoke(v2,'load_or_initialize_settings');assert v2.read(0x20000004,4)==base
  assert v2.read(0x2000072c,2)==16 and v2.read(0x20000740,2)==0xe0+k%10
 print('PASS: 64 relocated settings commit/reload scenarios; actual flash wrappers and primitives executed; motor/IO transitions mocked.')
 # End-to-end mutating UART commands, not just direct calls to commit.
 # Use a new VM per command so no motor-enable operation is executed.
 commands=[(0x84,[32],0x72c,32,2),(0x87,[1],0x736,1,1),
           (0x89,[0],0x738,0,1),(0x92,[4],0x5fa,4,2),
           (0xA1,[0x06,0x50],0x5ea,0x0650,2),
           (0xA2,[0,3],0x5ec,3,2),(0xA3,[0x06,0x40],0x5ee,0x0640,2),
           (0xA4,[0x01,0x9c],0x5f0,412,2)]
 def send(v,op,payload,bad=False):
  packet=[0xe0,op]+payload;packet.append((sum(packet)+int(bad))&255)
  for byte in packet:
   v.mmio[0x40000010]=1;v.mmio[0x40000000]=byte;invoke(v,'uart0_irq')
  if bad:return b''
  assert v.read(0x200006aa,1)==2,(op,v.read(0x200006aa,1))
  reply=[v.mmio[0x40000000]&255]
  for i in range(2):
   v.mmio[0x40000010]=2;invoke(v,'uart0_irq');reply.append(v.mmio[0x40000000]&255)
  v.mmio[0x40000010]=2;invoke(v,'uart0_irq')
  return bytes(reply)
 for op,payload,offset,expected,width in commands:
  for bad in [False,True]:
   v=FlashVM(flash,ins,bytearray(8192),{0x4000580c:0,0x40020d00:0x4000,0x40020c38:0,0x40000004:1,0x40000014:3},callbacks=callbacks)
   invoke(v,'load_or_initialize_settings');initial=v.read(0x20000000+offset,width)
   result=send(v,op,payload,bad)
   if bad:
    assert not v.operations and v.read(0x20000000+offset,width)==initial and v.read(0x200006aa,1)==0
   else:
    assert result==bytes([0xe0,1,0xe1]),(hex(op),result.hex())
    assert v.read(0x20000000+offset,width)==expected,(hex(op),hex(offset),expected,v.read(0x20000000+offset,width))
    assert len(v.operations)==29,(hex(op),len(v.operations))
    v2=FlashVM(v.flash,ins,bytearray(8192),{});invoke(v2,'load_or_initialize_settings');assert v2.read(0x20000000+offset,width)==expected
 print('PASS: 8 full mutating UART commands persisted and reloaded; 8 bad-checksum packets caused no flash writes. Control/IO callbacks mocked.')
 # Reproduce original failure-propagation gap, not a safety pass.
 # Low-level busy timeout returns 9; the legacy void write wrapper drops it.
 v=FlashVM(flash,ins,bytearray(8192),{});v.busy=True
 put(v.ram,0x6f4,0x1234,2);invoke(v,'factory_flash_write',0x7c00,0x200006f4,1)
 assert not v.operations and v.read(0x7c00,2)==65535
 print('KNOWN LIMITATION REPRODUCED: busy flash write returns through legacy wrapper without programming; caller receives no status.')
 # Stream full table with scripted inverse lookup; writes execute compiled low-level code.
 f=bytearray(flash);f[0x8000:]=bytes([0])*0x8000
 def inverse(v):v.r[0]=(v.r[0]*13+7)&65535
 v=FlashVM(f,ins,bytearray(8192),{},callbacks={addr('encoder_inverse_lookup'):('lookup',inverse)})
 invoke(v,'factory_generate_lut',limit=6000000)
 assert [op[1] for op in v.operations if op[0]=='erase']==list(range(0x8000,0x10000,512))
 assert [op[1] for op in v.operations if op[0]=='program']==list(range(0x8000,0x10000,2))+[0x7f96]
 assert v.flash[0x8000:]==b''.join(((i*13+7)&65535).to_bytes(2,'little') for i in range(16384))
 assert v.read(0x7f96,2)==1 and v.read(0x2000000b,1)==1
 assert v.flash[:0x7f96]==f[:0x7f96] and v.flash[0x7f98:0x8000]==f[0x7f98:0x8000]
 print(f'PASS: complete relocated LUT writer: 64 erases, 16,384 table writes, one valid marker; lookup values scripted; lowest modeled SP 0x{v.low_sp:08x}.')

def check_default_edit(elf,flash,syms):
 ins=load_ins(elf,0xc0,syms['__flash_code_end'])
 for saved in [False,True]:
  f=bytearray(flash)
  if saved:
   put(f,0x7c00,64,2);put(f,0x7c14,0xe7,2);put(f,0x7c36,1,2)
  v=FlashVM(f,ins,bytearray(8192),{});v.execute(syms['load_or_initialize_settings']&~1)
  assert v.read(0x2000072c,2)==(64 if saved else 32)
  assert v.read(0x20000740,2)==(0xe7 if saved else 0xe5)
