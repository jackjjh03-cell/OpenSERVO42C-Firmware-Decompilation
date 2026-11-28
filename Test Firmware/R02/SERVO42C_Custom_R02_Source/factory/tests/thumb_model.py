"""Limited instruction model adapted from the earlier differential tests. No physical hardware timing or concurrency."""
import re, subprocess
MASK=0xffffffff

def load_ins(elf,start,end):
 t=subprocess.check_output(['llvm-objdump','-d','--triple=thumbv6m-none-eabi',f'--start-address={start}',f'--stop-address={end}',str(elf)],text=True)
 out={}
 for line in t.splitlines():
  m=re.match(r'^\s*([0-9a-f]+):\s+([0-9a-f]{4}(?: [0-9a-f]{4})?)\s+([a-z][a-z0-9.]*)\s*(.*)$',line)
  if m:
   p,raw,op,args=m.groups();out[int(p,16)]=(len(raw.replace(' ',''))//2,op,args.split('@')[0].strip())
 return out
class Thumb:
 def __init__(self,flash,ins,ram,mmio):
  self.flash=flash;self.ins=ins;self.ram=bytearray(ram);self.mmio=dict(mmio);self.r=[0]*16;self.r[13]=0x20002000;self.r[14]=0xfffffffd;self.n=self.z=self.c=self.v=False;self.calls=[]
 def read(self,a,n):
  if 0<=a<=len(self.flash)-n:return int.from_bytes(self.flash[a:a+n],'little')
  if 0x20000000<=a<=0x20002000-n:return int.from_bytes(self.ram[a-0x20000000:a-0x20000000+n],'little')
  if 0x40000000<=a<0x50000000 and n==4:return self.mmio.get(a,0)
  raise AssertionError(('read',hex(a),n,hex(self.r[15])))
 def write(self,a,n,x):
  x &= (1<<(8*n))-1
  if 0x20000000<=a<=0x20002000-n:self.ram[a-0x20000000:a-0x20000000+n]=x.to_bytes(n,'little');return
  if 0x40000000<=a<0x50000000 and n==4:self.mmio[a]=x;return
  raise AssertionError(('write',hex(a),n,hex(self.r[15])))
 def reg(self,t):return {'sp':13,'lr':14,'pc':15}.get(t,int(t[1:]) if t.startswith('r') else -1)
 def val(self,t,p):
  if t.startswith('#'):return int(t[1:],0)&MASK
  if t=='pc':return p+4
  return self.r[self.reg(t)]
 def nz(self,x):self.n=bool(x&0x80000000);self.z=(x==0)
 def add(self,a,b,carry=0):
  s=a+b+carry;x=s&MASK;self.c=s>MASK;self.v=bool((~(a^b)&(a^x))&0x80000000);self.nz(x);return x
 def sub(self,a,b,borrow=0):
  x=(a-b-borrow)&MASK;self.c=a>=b+borrow;self.v=bool(((a^b)&(a^x))&0x80000000);self.nz(x);return x
 def addr(self,t,p):
  a=[x.strip() for x in t.strip('[]').split(',')];base=self.val(a[0],p)
  if a[0]=='pc':base&=~3
  return (base+(self.val(a[1],p) if len(a)>1 else 0))&MASK
 def step(self,p):
  if getattr(self,"legacy_addresses",False) and p==0x0110:
   self.r[0],self.r[1]=divmod(self.r[0],self.r[1]);return self.r[14]&~1
  if getattr(self,"legacy_addresses",False) and p in (0x470c,0x2090,0x0d3c,0x70dc) and p not in self.ins:
   # Calls are intentionally not emulated: commit observes the pending record;
   # leave the live state as-is, so both paths see the same stub behavior.
   self.calls.append((p,bytes(self.ram[0x6f4:0x72c]) if p==0x470c else bytes(self.ram[0x6a8:0x6b4]) if p==0x70dc else b''));return self.r[14]&~1
  if p not in self.ins:raise AssertionError(('unknown PC',hex(p)))
  size,op,arg=self.ins[p];nextp=p+size
  a=re.split(r',\s*(?![^\[]*\])(?![^\{]*\})',arg) if arg else []
  # split operands, keeping address and reglist commas inside brackets
  a=[x.strip() for x in a]
  if op in ('push','pop'):
   regs=sorted(self.reg(x.strip()) for x in arg.strip('{}').split(','))
   if op=='push':
    self.r[13]-=4*len(regs)
    for i,r in enumerate(regs):self.write(self.r[13]+i*4,4,self.r[r])
   else:
    for i,r in enumerate(regs):self.r[r]=self.read(self.r[13]+i*4,4)
    self.r[13]+=4*len(regs)
    if 15 in regs:nextp=self.r[15]&~1
  elif op in ('ldrsb','ldrsh'):
   n=1 if op=='ldrsb' else 2;x=self.read(self.addr(a[1],p),n);self.r[self.reg(a[0])]=(x-(1<<(8*n)) if x&(1<<(8*n-1)) else x)&MASK
  elif op in ('ldr','ldrb','ldrh','str','strb','strh'):
   n=1 if op.endswith('b') else 2 if op.endswith('h') else 4;addr=self.addr(a[1],p);r=self.reg(a[0]);
   if op.startswith('ldr'):self.r[r]=self.read(addr,n)
   else:self.write(addr,n,self.r[r])
  elif op in ('bl','b','beq','bne','bhi','bhs','blo','bpl','bmi','bgt','bge','blt','ble','bcs','bcc','bls'):
   cond={'b':True,'bl':True,'beq':self.z,'bne':not self.z,'bhi':self.c and not self.z,'bhs':self.c,'blo':not self.c,'bpl':not self.n,'bmi':self.n,'bgt':not self.z and self.n==self.v,'bge':self.n==self.v,'blt':self.n!=self.v,'ble':self.z or self.n!=self.v,'bcs':self.c,'bcc':not self.c,'bls':not self.c or self.z}[op]
   if cond:
    if op=='bl':self.r[14]=nextp|1
    nextp=int(arg.split()[0],0)&~1
  elif op=='bx':nextp=self.val(a[0],p)&~1
  elif op=='cmp':self.sub(self.val(a[0],p),self.val(a[1],p))
  elif op in ('mov','movs','uxtb','uxth','mvns'):
   x=self.val(a[1],p);x=x&255 if op=='uxtb' else x&65535 if op=='uxth' else (~x&MASK if op=='mvns' else x);self.r[self.reg(a[0])]=x
   if op=='mov' and self.reg(a[0])==15:nextp=x&~1
   if op in ('movs','mvns'):self.nz(x)
  elif op in ('add','adds','subs','sbcs','ands','orrs','bics','lsls','lsrs','asrs','muls'):
   r=self.reg(a[0]);x=self.val(a[0] if len(a)==2 else a[1],p);y=self.val(a[-1],p)
   if op=='add':z=(x+y)&MASK
   elif op=='adds':z=self.add(x,y)
   elif op=='subs':z=self.sub(x,y)
   elif op=='sbcs':z=self.sub(x,y,not self.c)
   elif op=='muls':z=(x*y)&MASK;self.nz(z)
   elif op in ('ands','orrs','bics'):
    z={'ands':x&y,'orrs':x|y,'bics':x&~y}[op]&MASK;self.nz(z)
   else:
    if not a[-1].startswith('#'):y&=255
    if y:
     self.c=bool((x>>(32-y if op=='lsls' else y-1))&1) if y<=32 else False
    z=((x<<y)&MASK if op=='lsls' else ((x if x<0x80000000 else x-0x100000000)>>y)&MASK if op=='asrs' else x>>y) if y<32 else 0;self.nz(z)
   if r==15:nextp=z&~1
   else:self.r[r]=z
  else:raise AssertionError(('unsupported',hex(p),op,arg))
  return nextp
 def run(self,p,arg=0):
  self.r[0]=arg&MASK
  for _ in range(20000):
   if p==0xfffffffc:return self.r[0]
   self.r[15]=p
   if getattr(self,"legacy_addresses",False) and p==0x15ec:
    a=self.r[14]&~1;p=(a+1+2*self.read(a+1+min(self.r[3],self.read(a,1)),1))&~1
   else:p=self.step(p)
  raise AssertionError('instruction limit')

class M3Thumb(Thumb):
 def __init__(self,*a,stubs=None,calibration=1):
  super().__init__(*a);self.stubs=stubs or {};self.calibration=calibration
 def step(self,p):
  if p in self.stubs:
   n=self.stubs[p];self.calls.append((p,tuple(self.r[:n])))
   if p==0x2e38:self.r[0]=self.calibration
   return self.r[14]&~1
  if getattr(self,"legacy_addresses",False) and p==0x0272:
   addr=self.r[0];n=0
   while self.read(addr+n,1):n+=1
   self.r[0]=n;return self.r[14]&~1
  if getattr(self,"legacy_addresses",False) and p==0x12f0:self.r[0]=self.read(self.r[0],2);return self.r[14]&~1
  if p not in self.ins:return super().step(p)
  size,op,arg=self.ins[p];a=[x.strip() for x in arg.split(',')];r=self.reg(a[0].strip("!")) if a else -1
  if op in ('sxth','sxtb'):
   n=16 if op=='sxth' else 8;x=self.val(a[1],p)&((1<<n)-1);self.r[r]=(x-(1<<n) if x&(1<<(n-1)) else x)&MASK
  elif op=='sub':self.r[r]=(self.val(a[0] if len(a)==2 else a[1],p)-self.val(a[-1],p))&MASK
  elif op=='blx':self.r[14]=(p+size)|1;return self.val(a[0],p)&~1
  elif op=='adcs':self.r[r]=self.add(self.r[r],self.val(a[-1],p),self.c)
  elif op=='eors':self.r[r]^=self.val(a[-1],p);self.nz(self.r[r])
  elif op=='tst':self.nz(self.val(a[0],p)&self.val(a[1],p))
  elif op=='rsbs':self.r[r]=self.sub(self.val(a[-1],p),self.val(a[1],p))
  elif op=='adr':self.r[r]=((p+4)&~3)+int(a[1][1:].split()[0],0)
  elif op in ('ldm','ldmia','stm','stmia'):
   base=self.reg(a[0].strip('!'));regs=sorted(self.reg(x.strip()) for x in arg.split('{')[1].strip('}').split(','));addr=self.r[base]
   for j,k in enumerate(regs):
    if op.startswith('ldm'):self.r[k]=self.read(addr+4*j,4)
    else:self.write(addr+4*j,4,self.r[k])
   if '!' in a[0]:self.r[base]=addr+4*len(regs)
  else:return super().step(p)
  return p+size
