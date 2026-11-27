#!/usr/bin/env python3
"""Execute compiled SysInfo navigation, formatter, UART and glyph paths on the host Thumb model."""
import argparse,pathlib,struct
from audit_factory import nm
from inspect_elf import read_application
from integration import VM,put
from thumb_model import load_ins
ROOT=pathlib.Path(__file__).resolve().parents[1]
BASE=0x20000000
TICK=0xe000e018

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('elf',type=pathlib.Path);p.add_argument('--profile',action='store_true');p.add_argument('--baseline',type=pathlib.Path);a=p.parse_args()
 syms=nm(a.elf);app,end=read_application(a.elf);flash=app+b'\xff'*(65536-len(app));ins=load_ins(a.elf,0xc0,syms['__flash_code_end']);addr=lambda n:syms[n]&~1
 ram=bytearray(8192);keys=[0,0];display=[]
 def lines(v):
  row=[]
  for ptr in v.r[:3]:
   b=bytearray()
   for i in range(11):
    x=v.read(ptr+i,1)
    if not x:break
    b.append(x)
   else:raise AssertionError('unterminated or oversized OLED row')
   assert len(b)<=10 and all(32<=c<=122 for c in b),b
   row.append(b.decode('ascii'))
  display.append(row)
 def gpio(v):
  port,pin=v.r[:2];v.r[0]=1-keys[pin-9] if port==0 and pin in (9,10) else 1
 cb={addr('display_three_lines'):('display',lines),addr('gpio_read_input_bit'):('gpio',gpio)}
 vm=VM(flash,ins,ram,{},callbacks=cb)
 vm.execute(addr('initialize_factory_data'))
 # Initial main table and user settings. Keep the whole legacy workspace snapshot
 # except legitimate key/menu changes to verify SysInfo never edits settings.
 put(vm.ram,0x62c,BASE+0x358,4);put(vm.ram,0,32000000,4);put(vm.ram,0x62a,1,1)
 # Begin at Exit, next reaches virtual SysInfo (20), then Enter opens categories.
 put(vm.ram,0x628,19,1)
 def poll(next=0,enter=0,step=1000):
  keys[:]=[next,enter];vm.mmio[TICK]=(vm.mmio.get(TICK,0)-step)&0xffffff;vm.execute(addr('menu_poll_buttons'),limit=100000)
 def press(i):
  poll(i==0,i==1);poll()
 press(0);assert vm.ram[0x628]==20
 assert any('SysInfo' in r for rows in display for r in rows),display[-3:]
 press(1);assert display[-1][0]=='*Credits',display[-1]
 catpages=[2,1,7,1,3,2,1 if a.profile else 2,1,2]+([1] if a.profile else [])
 names=['Credits','Hardware','Settings','PID','Motion','Encoder','Memory','Regs','Faults']+(['Perf'] if a.profile else [])
 n=0
 for variant in range(3):
  # exercise min, valid and maximal raw field values without touching menu or key state.
  for start,stop in [(0x5e2,0x5f0),(0x5f0,0x600),(0x607,0x622),(0x648,0x680),(0x680,0x6a8),(0x6c0,0x6f4),(0x72c,0x748),(0x8d8,0x8ec)]:
   vm.ram[start:stop]=bytes([0 if variant==0 else 255 if variant==2 else 17])*(stop-start)
  snapshot=bytes(vm.ram[0x5e2:0x622]+vm.ram[0x648:0xa54]);
  for c,pc in enumerate(catpages):
   assert display[-1][0]=='*'+names[c],(c,display[-1])
   press(1)
   for j in range(pc):
    assert len(display[-1])==3
    n+=1
    press(0)
   press(1);assert display[-1][0]=='*'+names[c]
   press(0)
  assert display[-1][0]=='*Exit'
  press(0);assert display[-1][0]=='*Credits'
  assert snapshot==bytes(vm.ram[0x5e2:0x622]+vm.ram[0x648:0xa54]),'read-only navigation modified legacy motor/settings RAM'
 # live refresh at half a second (16 million cycles @ 32MHz); then exit and wrap main menu
 press(1);before=len(display);poll(step=8000000);poll(step=8100000);assert len(display)>before,'no refresh';press(1)
 for i in range(len(catpages)):press(0)
 press(1);assert vm.mmio[0xe000e010]==0
 poll();assert display[-1][0]=='*SysInfo';press(0);assert vm.ram[0x628]==0
 # Exercise real fault override while the custom screen owns SysTick.
 vm.execute(addr('sysinfo_open'));poll();put(vm.ram,0x737,1,1);put(vm.ram,0x649,1,1)
 saved_cb=vm.callbacks.copy()
 for name in ['fault_indicator_write','factory_oled_command','update_multiturn_angle','reset_position_state','step_input_enable','factory_refresh_mode','factory_delay_ms','render_telemetry']:
  vm.callbacks[addr(name)]=(name,lambda v:None)
 keys[:]=[0,1];vm.execute(addr('factory_foreground_ui'))
 assert ['Wrong',' Protect!','Enter..'] in display and vm.mmio[0xe000e010]==0
 vm.callbacks=saved_cb;put(vm.ram,0x62c,BASE+0x358,4);keys[:]=[0,0];put(vm.ram,0x5dd,0,1)
 if a.profile:
  # Navigate to Perf and run actual wrapper with a scripted 100,000-cycle
  # motion handler. Verify all displayed results plus stop sampling on delay.
  vm.execute(addr('sysinfo_open'));poll()
  for _ in range(9):press(0)
  press(1);assert display[-1][0]=='Sampling'
  def motion(v):v.mmio[TICK]=(v.mmio[TICK]-100000)&0xffffff
  vm.callbacks[addr('motion_timer_irq')]=('motion',motion)
  for _ in range(10):vm.execute(addr('sysinfo_profile_irq'))
  poll(step=7500000);poll(step=7500000)
  assert display[-1]==['Hz 20','Load 6','C 000186A0'],display[-1]
  vm.execute(addr('sysinfo_profile_suspend'));vm.mmio[0xe000e010]=0;poll();assert display[-1][0]=='Sampling'
  vm.callbacks=saved_cb;vm.execute(addr('sysinfo_cancel'))
  print('PASS: profiling wrapper, scripted IRQ count/load/peak and suspension on timer reuse.')
 # explicit cancellation stops timer and preserves runtime settings.
 vm.execute(addr('sysinfo_open'));poll();vm.execute(addr('sysinfo_cancel'));assert vm.mmio[0xe000e010]==0
 print(f'PASS: {n} OLED data pages, every category, three value ranges, main-menu append/wrap, exit, live refresh, fault priority and explicit cancellation.')
 # Test actual dispatcher with custom query and every allowed/invalid selector, plus packet parser.
 # RAM fields initialized by the test; read service has no side effects.
 offsets=[(0x735,1),(0x73c,4),(0x740,2),(0x72c,2),(0x5e4,2),(0x730,1),
  (0x734,1),(0x737,1),(0x738,1),(0x736,1),(0x72e,2),(0x5e8,2),(0x5f0,2),(0x746,2),(0x732,2),
  (0x5f6,1),(0x5fa,2),(0x5fc,1),(0x5f8,2),(0x742,2),(0x744,2),(0x5ea,2),(0x5ec,2),(0x5ee,2),
  (0x5e2,1),(0x13,1),(0x649,1),(0x66c,4),(0x680,4),(0x688,4),(0x618,2),(0x607,1),(0x6dc,4),
  (0x64c,2),(0x620,1),(0xb,1),(0x8dc,2),(0x8de,2),(0x690,4),(0x648,1),(0x649,1),(0x737,1),
  (0x8e4,4),(0x8e8,4),(0x6c2,2)]
 for idx in range(256):
  w=VM(flash,ins,vm.ram,{},callbacks={addr('factory_start_reply'):('reply',lambda v:None)})
  # Distinct offset-dependent values avoid treating a wrong field as correct.
  for off,size in offsets:put(w.ram,off,(off*0x10203+idx)&0xffffffff,size)
  put(w.ram,0x740,0xe0,2);w.ram[0x6a8:0x6bc]=bytes(20)
  frame=bytes([0xe0,0x8d,idx,(0xe0+0x8d+idx)&255])
  # The factory parser treats an embedded device address as resync for 0x8D.
  # 0xE0 is not a supported selector; choose a different address for this case
  # (and for checksum collision). The host utility only queries 0..44.
  address=0xe1 if idx==0xe0 else 0xe0
  put(w.ram,0x740,address,2);frame=bytes([address,0x8d,idx,(address+0x8d+idx)&255])
  if frame[-1]==address or idx==address:
   # Unsupported selector/checksum collision is tested at dispatcher boundary.
   w.ram[0x6ab:0x6af]=frame
  else:
   for b in frame:
    w.r[:3]=[BASE+0x6a8,address,b];w.execute(addr('uart_receive_byte'))
   assert w.ram[0x6a9]==1,(idx,frame.hex())
  pre=bytes(w.ram[0x5e2:0x6a8]+w.ram[0x6bc:0xa54]);w.execute(addr('uart_dispatch'))
  assert w.ram[0x6aa]==5
  payload=bytes(w.ram[0x6b5:0x6b9][::-1]);value=int.from_bytes(payload,'big')
  want=int.from_bytes(w.ram[offsets[idx][0]:offsets[idx][0]+offsets[idx][1]],'little') if idx<45 else 0xffffffff
  assert value==want,(idx,payload.hex(),hex(want))
  assert w.ram[0x6b4]==(address+sum(payload))&255
  assert pre==bytes(w.ram[0x5e2:0x6a8]+w.ram[0x6bc:0xa54])
 print('PASS: 256 custom diagnostic selector/response cases, checksum and read-only state.')
 if a.baseline:
  bs=nm(a.baseline);ba,_=read_application(a.baseline);bi=load_ins(a.baseline,0xc0,bs['__flash_code_end']);oldfont=ba[bs['oled_font']:bs['oled_font']+1278];newmap=app[syms['oled_glyph_map']:syms['oled_glyph_map']+91];retained=0
  fallback=newmap[ord('?')-32]
  for char in range(32,123):
   # Only fallback glyphs differ; compare all others to the actual stock OLED renderer.
   if newmap[char-32]==fallback and char!=ord('?'):continue
   rows=[bytes([char,0])]*3
   def render(f,ii,s):
    rr=bytearray(8192)
    for i,row in enumerate(rows):rr[0x1700+i*16:0x1700+i*16+2]=row
    v=VM(f,ii,rr,{});v.r[:3]=[BASE+0x1700+i*16 for i in range(3)];v.execute(s['oled_render_text']&~1,limit=10000);return v.ram[0x8ec:0x8ec+360]
   assert render(ba,bi,bs)==render(app,ins,syms),chr(char);retained+=1
  print(f'PASS: {retained} retained glyphs rendered identically to stock in all three rows (framebuffer comparison).')
 print(f'PASS: candidate application extent {end}, reserved flash starts at 0x7a00.')
if __name__=='__main__':main()
