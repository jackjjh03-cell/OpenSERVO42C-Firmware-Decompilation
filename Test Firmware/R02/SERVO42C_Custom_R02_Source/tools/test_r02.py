#!/usr/bin/env python3
"""R02 targeted checks. Executes linked Thumb instructions with modeled peripherals."""
import argparse, pathlib, tempfile
from audit_factory import nm, wrap_original
from inspect_elf import read_application
from check_backup import read_backup
from integration import VM, put, Checkpoint
from persistence import FlashVM
from thumb_model import load_ins
BASE=0x20000000
TICK=0xe000e018

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('elf',type=pathlib.Path);p.add_argument('backup',type=pathlib.Path);a=p.parse_args()
 app,end=read_application(a.elf);backup=read_backup(a.backup);flash=app+backup[0x7a00:];s=nm(a.elf);addr=lambda n:s[n]&~1;ins=load_ins(a.elf,0xc0,s['__flash_code_end'])
 def invoke(v,n,*args,limit=200000):
  v.r[:len(args)]=args;v.execute(addr(n),limit=limit);return v.r[0]
 def rows(v):
  result=[]
  for ptr in v.r[:3]:
   b=bytearray()
   for i in range(11):
    c=v.read(ptr+i,1)
    if not c:break
    b.append(c)
   else:raise AssertionError(('oversized display',b))
   result.append(b.decode('ascii'))
  return result
 def set_cb(v,name,fn):v.callbacks[addr(name)]=(name,fn)
 # Identity: preserve actual enrollment marker and identity word, no bypass.
 assert backup[0x7bfc:0x7c00]!=b'\xff'*4 and backup[0x7ffc:0x7ffe]!=b'\x82\xcc'
 assert flash[0x7a00:]==backup[0x7a00:]
 stored=int.from_bytes(backup[0x7bfc:0x7c00],'little')
 class IdentityVM(VM):
  def __init__(self,*args,result,**kw):super().__init__(*args,**kw);self.result=result;self.trace=[];self.rom={0x00100e78:0x12345678,0x00100e7c:0x87654321,0x00100e84:0x11223344,0x00100e88:0xaabbccdd}
  def read(self,a,n):
   if a in self.rom and n==4:return self.rom[a]
   if a==0x40021400 and self.mmio.get(a,0)&1:
    self.mmio[a]&=~1
    for i in range(4):self.mmio[0x40021410+i*4]=self.result if i==0 else 0x01010101*i
   return super().read(a,n)
  def write(self,a,n,v):
   self.trace.append((a,n,v)) if 0x40021400<=a<0x40021440 else None
   return super().write(a,n,v)
 with tempfile.TemporaryDirectory() as td:
  oldelf=wrap_original(backup,pathlib.Path(td));oi=load_ins(oldelf,0xc0,0x7a00)
  for expected in (0,1):
   out=stored if expected else stored^1;results=[]
   for legacy,f,ii,entry in [(True,backup,oi,0x2c2c),(False,flash,ins,addr('factory_identity_check'))]:
    v=IdentityVM(f,ii,bytearray(8192),{},legacy=legacy,result=out);v.execute(entry);assert v.r[0]==expected
    results.append((v.trace,v.mmio))
   assert results[0]==results[1],results
 print('PASS: original/rebuilt device identity routine, matching and mismatching modeled transform; hardware register writes match; original reserved bytes retained, no enrollment marker.')
 # Startup fault: real interlock, outputs, UART shutdown, exact delay calls, menu entry.
 for status in (1,2,3,4):
  for early in ('none','menu','enter'):
   mm={0x40000c30:0x1000,0x40000d30:0x1000,0x40021014:0xf0000,0x400024b4:0x20,0x40000004:3,0x4000580c:0x401,0x40003000:1}
   v=VM(flash,ins,bytearray(8192),mm);invoke(v,'initialize_factory_data');invoke(v,'load_or_initialize_settings')
   put(v.ram,0x620,status,1);put(v.ram,0x607,1,1);put(v.ram,0x13,1,1);put(v.ram,0x5e2,1,1)
   warnings=[];delays=[]
   set_cb(v,'boot_banner',lambda v:None);set_cb(v,'startup_prepare',lambda v:v.r.__setitem__(0,3));set_cb(v,'display_three_lines',lambda v:warnings.append(rows(v)))
   def delay(v):delays.append(v.r[0])
   def key(v):v.r[0]=int(not (len(delays)>=12 and ((early=='menu' and v.r[:2]==[64,4]) or (early=='enter' and v.r[:2]==[0,10]))))
   def stop(v):raise Checkpoint
   set_cb(v,'factory_delay_ms',delay);set_cb(v,'gpio_read_input_bit',key);set_cb(v,'factory_foreground_ui',stop)
   try:invoke(v,'factory_startup_diagnostics',limit=400000)
   except Checkpoint:pass
   else:raise AssertionError('fault startup returned to motor startup')
   assert sum(delays)==(10000 if early=='none' else 120),delays
   assert len(warnings)==1 and warnings[0][0]==('Magnet' if status<=3 else 'Encoder'),warnings
   assert v.read(s['magnet_lock'],1)==1 and v.ram[0x648]==3 and v.ram[0x5e2]==0 and v.ram[0x607]==0 and v.ram[0x13]==0
   assert v.read(BASE+0x62c,4)==BASE+0x358 and v.ram[0x62a]==1
   assert v.mmio[0xe000e180]&0x01000072==0x01000072
   assert not (v.mmio[0x40000c30]&0x1000 or v.mmio[0x40000d30]&0x1000 or v.mmio[0x40021014]&0xf0000 or v.mmio[0x40000004]&3)
   for n in ('initialize_motion_tick','initialize_step_inputs','origin_return','factory_apply_uart'):assert n not in v.calls
 print('PASS: 12 startup encoder-fault paths (4 status codes x timeout/Menu/Enter), 10-second scripted timeout, motor/UART shutdown and full menu entry.')
 # Locked settings use actual persistence; no unsafe action can resume motor.
 v=FlashVM(flash,ins,bytearray(8192),{0x4000580c:1});invoke(v,'initialize_factory_data');invoke(v,'load_or_initialize_settings');v.write(s['magnet_lock'],1,1);put(v.ram,0x648,3,1)
 invoke(v,'menu_select_table',BASE+0x358);put(v.ram,0x62a,1,1)
 for name,value in [('menu_control_mode',0),('menu_enable',2),('menu_control_mode',2),('menu_uart_baud',5),('menu_current',3),('menu_microsteps',4),('menu_protection',1)]:
  invoke(v,name,value)
  assert v.read(s['magnet_lock'],1)==1 and not v.ram[0x5e2] and not v.ram[0x607]
  assert not v.mmio.get(0x40000c30,0)&0x1000 and not v.mmio.get(0x40000d30,0)&0x1000 and not v.mmio.get(0x40000004,0)&3
 assert v.operations and all(0x7c00<=x[1]<0x7e00 for x in v.operations)
 for name in ['read_enable_state','factory_calibrate','factory_menu_set_origin','factory_menu_goto_origin','set_timer_output_field','factory_resume_control','factory_refresh_mode','factory_apply_uart']:
  invoke(v,name,0);assert not v.mmio.get(0x40000c30,0)&0x1000 and not v.mmio.get(0x40000d30,0)&0x1000 and not v.mmio.get(0x40000004,0)&3 and not v.ram[0x5e2]
 assert v.flash[:0x7c00]==flash[:0x7c00] and v.flash[0x7e00:]==flash[0x7e00:]
 display=[];set_cb(v,'display_three_lines',lambda v:display.append(rows(v)))
 invoke(v,'render_telemetry');assert display[-1]==['Motor lock','RPM --','Menu']
 keys=[0,0,0]
 def gpio(v):
  port,pin=v.r[:2];v.r[0]=1-(keys[0] if port==64 and pin==4 else keys[1] if port==0 and pin==9 else keys[2] if port==0 and pin==10 else 0)
 set_cb(v,'gpio_read_input_bit',gpio);put(v.ram,0x5dd,0,1);put(v.ram,0x628,19,1)
 keys[1]=1;invoke(v,'factory_foreground_ui');keys[1]=0;invoke(v,'factory_foreground_ui');assert v.ram[0x628]==20
 keys[2]=1;invoke(v,'factory_foreground_ui');keys[2]=0;invoke(v,'factory_foreground_ui');assert display[-1][0]=='*Credits'
 keys[0]=1;invoke(v,'factory_foreground_ui');keys[0]=0;invoke(v,'factory_foreground_ui');assert display[-1][0]=='Motor lock'
 print('PASS: real locked settings commits, mode changes, enable/baud changes, blocked motion actions, SysInfo and return to lock display; reserved non-settings regions unchanged.')
 # RPM from real compiled telemetry with scripted SysTick/encoder samples.
 v=VM(flash,ins,bytearray(8192),{});display=[];set_cb(v,'display_three_lines',lambda v:display.append(rows(v)))
 put(v.ram,0,32000000,4);put(v.ram,0x735,2,1);put(v.ram,0x5e2,1,1);put(v.ram,0xb,1,1)
 def render():invoke(v,'render_telemetry');return display[-1][2]
 def sample(delta,expected,start):
  invoke(v,'rpm_reset');put(v.ram,0x690,start,4);assert render()=='RPM --'
  v.mmio[TICK]=(v.mmio[TICK]-8000000)&0xffffff;put(v.ram,0x690,start+delta//2,4);assert render()=='RPM --'
  v.mmio[TICK]=(v.mmio[TICK]-8000000)&0xffffff;put(v.ram,0x690,start+delta,4);assert render()==f'RPM {expected}',display[-1]
 for delta,expected in [(0,0),(32768,60),(65536,120),(-65536,120),(1638400,3000)]:
  for start in (0,0xffff8000):sample(delta,expected,start)
 invoke(v,'rpm_rebase');assert render()=='RPM --';put(v.ram,0x5e2,0,1);assert render()=='RPM --'
 put(v.ram,0x680,123,4);invoke(v,'rpm_toggle');assert render()=='123clk';invoke(v,'rpm_toggle');assert render()=='RPM --'
 # Actual main-view Next button is edge triggered, not a repeated toggle on hold.
 keys=[0,0,0];set_cb(v,'gpio_read_input_bit',gpio);put(v.ram,0x5dd,0,1);put(v.ram,0x62a,0,1)
 keys[1]=1;invoke(v,'factory_foreground_ui');assert display[-1][2]=='123clk'
 invoke(v,'factory_foreground_ui');assert display[-1][2]=='123clk'
 keys[1]=0;invoke(v,'factory_foreground_ui');keys[1]=1;invoke(v,'factory_foreground_ui');assert display[-1][2]=='RPM --'
 glyph=app[s['oled_glyph_map']:s['oled_glyph_map']+91];fallback=glyph[ord('?')-32]
 assert all(glyph[ord(c)-32]!=fallback for c in 'Motor lockRPM --Menu123clk')
 print('PASS: 10 measured RPM model samples (zero, forward/reverse, 32-bit wrap, 3000 RPM), rebase, drive-off, actual Next-key pulse/RPM toggle and retained display glyphs.')
 print('Hardware, actual encoder timing, and motor power-stage behavior remain unverified.')
if __name__=='__main__':main()
