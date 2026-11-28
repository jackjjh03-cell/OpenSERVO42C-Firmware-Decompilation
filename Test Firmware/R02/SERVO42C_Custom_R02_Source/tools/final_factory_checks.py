#!/usr/bin/env python3
"""Final targeted comparisons against original instructions, with scripted hardware."""
import argparse, pathlib, tempfile, sys, hashlib
from audit_factory import ROOT, nm, wrap_original
from inspect_elf import read_application
from check_backup import read_backup
from integration import VM, put
from persistence import FlashVM
from thumb_model import load_ins

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('elf',type=pathlib.Path);p.add_argument('backup',type=pathlib.Path);a=p.parse_args()
 old=read_backup(a.backup);app,end=read_application(a.elf);s=nm(a.elf);addr=lambda n:s[n]&~1
 assert hashlib.sha256(old[:0x7a00]).hexdigest()=='1a64209c9cb46679f78628b8eb82cddcb5ef78972b9f6699a9b41aeb12610595'
 new=app+old[0x7a00:];print('Application SHA256:',hashlib.sha256(app).hexdigest(),flush=True)
 with tempfile.TemporaryDirectory() as tmp:
  oe=wrap_original(old,pathlib.Path(tmp));oi=load_ins(oe,0xc0,0x7a00);ni=load_ins(a.elf,0xc0,s['__flash_code_end'])
  def pair(r,mm,callbacks,cls=VM,of=old,nf=new):
   return [cls(f,ii,r,mm,legacy=l,callbacks={(q if l else addr(n)):(n,fn) for n,q,fn in callbacks}) for f,ii,l in [(of,oi,True),(nf,ni,False)]]
  def nop(v):pass
  def mode(v):v.r[0]%=3
  cb=[(n,q,mode if n=='factory_apply_mode' else nop) for n,q in [('factory_pause_control',0x5874),('factory_resume_control',0x58a0),('factory_apply_mode',0x570c),('factory_apply_direction',0x583c),('factory_apply_uart',0x5bd4)]]
  for k in range(64):
   slot=k%8;page=bytearray(b'\xff'*512)
   for i in range(slot+1):put(page,i*64,16,2);put(page,i*64+54,1,2)
   scenario=(k//8)%4
   if scenario==1 and slot<7:put(page,(slot+1)*64,32,2)
   if scenario==2:put(page,slot*64+54,0,2)
   of=bytearray(old);nf=bytearray(new);of[0x7c00:0x7e00]=nf[0x7c00:0x7e00]=page
   r=bytearray(8192);put(r,4,0x7c00+slot*64,4);put(r,0x5d9,k%5,1);put(r,0x648,k%3,1)
   for o,x in [(0x6f4,16),(0x710,1200),(0x712,600),(0x714,25),(0x708,0xe0+k%10)]:put(r,o,x,2)
   put(r,0x6fd,k%3,1)
   v,w=pair(r,{0x4000580c:k%2},cb,FlashVM,of,nf)
   v.execute(0x470c,limit=200000);w.execute(addr('factory_commit'),limit=200000)
   assert (v.ram[:0xa54],v.mmio,v.operations,v.calls,v.flash[0x7a00:])==(w.ram[:0xa54],w.mmio,w.operations,w.calls,w.flash[0x7a00:]),('commit',k)
  print('PASS: 64 differential settings commits, including dirty slots, invalid markers, full-page rollover; workspace, ordered NOR operations, MMIO, callbacks and reserved flash compared.',flush=True)
  for op,q in [('flash_erase_page',0x1188),('flash_program_halfword',0x1260)]:
   for case in ('normal','busy','refused','outside'):
    r=bytearray(8192);of=bytearray(old);nf=bytearray(new);of[0x7c00:0x7e00]=nf[0x7c00:0x7e00]=b'\x5a'*512
    v,w=pair(r,{},[],FlashVM,of,nf)
    for vm,entry in [(v,q),(w,addr(op))]:
     vm.busy=case=='busy';vm.refuse_mode=case=='refused';vm.r[:2]=[0x10000 if case=='outside' else 0x7c40,0x1234];vm.execute(entry)
    assert (v.r[0],v.operations,v.flash[0x7a00:],v.mmio)==(w.r[0],w.operations,w.flash[0x7a00:],w.mmio),(op,case)
  print('PASS: 8 differential flash primitive cases, including timeout and rejected mode; modeled MMIO and flash effects.',flush=True)
  names={'set_timer_output_field':0x34a4,'clock_adc_set_bit19':0x3b9c,'timer2_resume_irq':0x5918,'timer2_pause_irq':0x588c,'clock_adc_stop':0x3b70,'clear_timer_output_field':0x3488,'reset_compare_values':0x342c,'factory_delay_100us':0x2d98,'display_three_lines':0x4884,'origin_failure_message':0x4d94,'fault_indicator_write':0x3f28}
  def text(v,p):
   b=bytearray()
   for i in range(64):
    x=v.read(p+i,1)
    if not x:return bytes(b)
    b.append(x)
   raise AssertionError('unterminated')
  cases=0
  for modeval in (0,1,2):
   for direction in (0,1):
    for error in (-101,-100,-18,0,18,100,101,500):
     for startup in (0,1):
      # Large pulse scale bounds motion steps. Script a settled encoder at the
      # first feedback poll; no elapsed-time or real motor response claim.
      r=bytearray(8192);r[0x735]=1;r[0x5f6]=modeval;r[0x5fc]=direction
      put(r,0x5f8,2000,2);put(r,0x5fa,(cases//2)%5,2);put(r,0x5e8,25,2);put(r,0x654,65536,4);put(r,0x690,2000-error,4);put(r,0x680,0xffffffff if cases%2 else 0,4)
      results=[]
      for legacy,f,ii in [(True,old,oi),(False,new,ni)]:
       events=[]
       def cb(n):
        def fn(v):
         if n=='display_three_lines':events.append((n,tuple(text(v,x) for x in v.r[:3])))
         elif n=='factory_delay_100us':events.append((n,v.r[0]));put(v.ram,0x690,2000,4)
         elif n=='fault_indicator_write':events.append((n,v.r[0]))
         else:events.append((n,))
        return fn
       v=VM(f,ii,r,{},legacy=legacy,callbacks={(q if legacy else addr(n)):(n,cb(n)) for n,q in names.items()});v.r[0]=startup;v.execute(0x4dc8 if legacy else addr('origin_return'),limit=2000000);results.append((v.r[0],v.ram[:0xa54],events))
      assert results[0]==results[1],('origin',modeval,direction,error,startup,results[0][0],results[1][0],results[0][2],results[1][2]);cases+=1
  print(f'PASS: {cases} differential origin-return cases with scripted settling; return, workspace, display and callback sequence compared. Delay timing and physical response not modeled.',flush=True)
  # Compare the complete two-pass calibration orchestration and wrapping
  # logic. Synthetic encoder samples model motion, not physical calibration.
  names={'factory_pause_control':0x5874,'display_three_lines':0x4884,'reset_phase_state':0x3cac,'set_timer_output_field':0x34a4,'set_phase_and_value':0x3470,'clock_adc_set_bit19':0x3b9c,'factory_delay_ms':0x2de8,'encoder_trimmed_mean':0x5a38,'calibration_step_forward':0x3534,'calibration_step_reverse':0x34c0,'clock_adc_stop':0x3b70,'calibration_record_bounds':0x279c,'menu_select_table':0x4574,'fault_indicator_write':0x3f28,'reset_compare_values':0x342c,'factory_resume_control':0x58a0}
  for k in range(8):
   r=bytearray(8192);put(r,0x5e8,25 if k%2 else 50,2)
   forward=[(i*82+k*1234)%16384 for i in range(200)]
   reverse=[(forward[i]+(-20 if k%2 else 20))%16384 for i in reversed(range(200))]
   samples=forward+reverse;results=[]
   for legacy,f,ii in [(True,old,oi),(False,new,ni)]:
    events=[];index=[0]
    def cb(n):
     def fn(v):
      if n=='display_three_lines':events.append((n,tuple(text(v,x) for x in v.r[:3])))
      elif n=='encoder_trimmed_mean':
       events.append((n,v.r[0]));v.r[0]=samples[index[0]];index[0]+=1
      elif n in ('set_phase_and_value','factory_delay_ms','menu_select_table','fault_indicator_write'):
       events.append((n,tuple(v.r[:2 if n=='set_phase_and_value' else 1])))
      else:events.append((n,))
     return fn
    v=VM(f,ii,r,{0x4000580c:k%2},legacy=legacy,callbacks={(q if legacy else addr(n)):(n,cb(n)) for n,q in names.items()});v.execute(0x54cc if legacy else addr('calibration_run'),limit=300000);assert index[0]==400;results.append((v.r[0],v.ram[:0xa54],events))
   assert results[0]==results[1],('calibration',k,results[0][0],results[1][0],[(i,x,y) for i,(x,y) in enumerate(zip(results[0][1],results[1][1])) if x!=y][:10])
  print('PASS: 8 differential complete calibration sweep sequences, 400 scripted encoder samples each; averaging, wrap logic, workspace and callback order compared. Physical step, flash, and electrical operations mocked.',flush=True)
if __name__=='__main__':main()
