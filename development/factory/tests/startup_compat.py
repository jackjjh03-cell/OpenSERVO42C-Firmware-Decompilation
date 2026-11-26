"""Relocated startup diagnostic and foreground state-machine comparison.
Physical peripherals and time are scripted; not hardware initialization proof.
"""
from integration import VM,put,ROOT
from thumb_model import load_ins
import random,collections

MAP={'factory_startup_diagnostics':(0xe20,0),'boot_banner':(0x2fe0,0),'menu_select_table':(0x4574,1),'menu_request_redraw':(0x4568,0),'reset_compare_values':(0x342c,0),'factory_apply_uart':(0x5bd4,0),'initialize_motion_tick':(0x5b84,0),'origin_return':(0x4dc8,1),'initialize_step_inputs':(0x3cc8,0),'startup_prepare':(0x1df0,0),'gpio_read_input_bit':(0x135c,2),'load_calibration_knots':(0x267c,0),'factory_delay_ms':(0x2de8,1),'display_three_lines':(0x4884,3),'factory_foreground_ui':(0x1b28,0),'fault_indicator_write':(0x3f28,1),'factory_oled_command':(0x4d84,1),'update_multiturn_angle':(0x27e8,0),'reset_position_state':(0x20e4,0),'step_input_enable':(0x3e54,0),'factory_refresh_mode':(0x2090,0),'render_telemetry':(0x3220,0),'menu_poll_buttons':(0x311c,0),'factory_timer_reload':(0x908,2),'factory_timer_start':(0x924,1),'factory_timer_stop':(0x93c,1)}

def check_startup(elf,flash,syms):
 oldelf=ROOT.parent/'build/firmware.elf';oldbin=ROOT.parent/'reference/factory.bin'
 if not oldelf.exists() or not oldbin.exists():print('SKIP: startup factory comparison requires private fixtures.');return
 oi=load_ins(oldelf,0xc0,0x7a00);ni=load_ins(elf,0xc0,syms['__flash_code_end']);oldflash=oldbin.read_bytes();rng=random.Random(0x42c2020)
 addr=lambda n:syms[n]&~1
 def compare(entry,r,returns,stubs):
  results=[]
  for legacy,f,ii in ((True,oldflash,oi),(False,flash,ni)):
   events=[];q={name:collections.deque(vals) for name,vals in returns.items()}
   def callback(name,n):
    def run(v):
     args=tuple(v.r[:n]);
     if name=='display_three_lines':
      def string(p):
       b=bytearray()
       for i in range(80):
        x=v.read(p+i,1)
        if not x:return bytes(b)
        b.append(x)
       raise AssertionError('unterminated diagnostic text')
      args=tuple(string(p) for p in args)
     events.append((name,args))
     if name=='factory_foreground_ui':v.ram[0x735]=0
     if name in q:
      assert q[name],('script exhausted',name);v.r[0]=q[name].popleft()&0xffffffff
    return run
   cb={ (MAP[name][0] if legacy else addr(name)):(name,callback(name,MAP[name][1])) for name in stubs }
   v=VM(f,ii,r,{},legacy=legacy,callbacks=cb);v.execute(MAP[entry][0] if legacy else addr(entry),limit=100000)
   results.append((v.ram[:0xa54],v.mmio,events))
  assert results[0]==results[1],(entry,[(hex(i),a,b) for i,(a,b) in enumerate(zip(results[0][0],results[1][0])) if a!=b][:15],results[0][2],results[1][2])
 stubs=['boot_banner','menu_select_table','menu_request_redraw','reset_compare_values','factory_apply_uart','initialize_motion_tick','origin_return','initialize_step_inputs','startup_prepare','gpio_read_input_bit','load_calibration_knots','factory_delay_ms','display_three_lines','factory_foreground_ui']
 # Include each diagnostic outcome, repeated phase/motor failure, all encoder status strings.
 for k in range(32):
  r=bytearray(rng.getrandbits(0xa54*8).to_bytes(0xa54,'little')+bytes(8192-0xa54));put(r,0x735,1,1);put(r,0x620,(k//4)%5,1)
  startup=[[0],[4,4,1,0],[3,0],[2,0]][k%4]
  compare('factory_startup_diagnostics',r,{'startup_prepare':startup,'gpio_read_input_bit':[1,0,0,1],'load_calibration_knots':[0,1]},stubs)
 print('PASS: 32 relocated startup diagnostic paths; display messages, call sequence, workspace and error recovery compared with factory; peripherals mocked.',flush=True)
 # Foreground protection acknowledgement, sleep/wake and button behavior.
 stubs=['fault_indicator_write','factory_oled_command','display_three_lines','update_multiturn_angle','reset_position_state','step_input_enable','factory_refresh_mode','menu_select_table','factory_delay_ms','boot_banner','render_telemetry','menu_poll_buttons','gpio_read_input_bit','factory_timer_reload','factory_timer_start','factory_timer_stop']
 for k in range(256):
  r=bytearray(rng.getrandbits(0xa54*8).to_bytes(0xa54,'little')+bytes(8192-0xa54))
  for off,val,n in [(0x737,k%2,1),(0x649,(k//2)%2,1),(0x5dc,(k//4)%2,1),(0x5d8,1,1),(0x5dd,(k//8)%8,1),(0x5de,[0,9,10,65535][k%4],2),(0x5e0,[0,1,4095,4096][(k//4)%4],2),(0x735,(k//8)%3,1),(0x736,(k//24)%2,1),(0x62a,(k//48)%2,1),(0x62c,[0,0x20000358,0x2000059c][k%3],4)]:put(r,off,val,n)
  compare('factory_foreground_ui',r,{'gpio_read_input_bit':[k%2,(k//2)%2,(k//3)%2,1,0,1,0,1]},stubs)
 print('PASS: 256 relocated foreground UI/protection/sleep/wake cases compared with factory. Physical drive and GPIO effects mocked.',flush=True)
