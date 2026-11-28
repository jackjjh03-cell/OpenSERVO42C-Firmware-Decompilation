"""Differential test of complete board initialization, including peripheral helpers.
Hardware register reads/writes are modeled as ordinary storage, not electrical
signals or write-one-to-clear behavior. Clock frequency and delay are scripted.
"""
import random, struct
from integration import VM
from thumb_model import load_ins

POINTS={'initialize_indicator':0x3f84,'initialize_display_bus':0x4c8c,
        'initialize_encoder_bus':0x505c,'initialize_motor_timers':0x35a8,
        'gpio_config_pin':0x137c}

class BoardVM(VM):
    def __init__(self,*args,points,**kwargs):
        super().__init__(*args,**kwargs)
        self.points=points;self.trace=[];self.pins=[];self.entries=[]
    def step(self,p):
        if p in self.points:
            name=self.points[p]
            if name=='gpio_config_pin':
                fields=tuple(self.read(self.r[2]+i,1) for i in range(6))
                self.pins.append((self.r[0],self.r[1],fields))
            else:self.entries.append((name,self.r[2]))
        return super().step(p)
    def write(self,a,n,v):
        if a>=0x40000000:self.trace.append((a,n,v&((1<<(8*n))-1)))
        return super().write(a,n,v)

# GPIO fields 0..4 are interpreted by the original 0x16AC helper as
# enabled iff equal to 1; field 5 contributes its low bit to the register.
def pin_effects(pins):
    return [(p,n,tuple(int(x==1) for x in f[:5])+(f[5]&1,)) for p,n,f in pins]

def compare_board_init(elf,flash,syms,oldelf,backup):
    oi=load_ins(oldelf,0xc0,0x7a00);ni=load_ins(elf,0xc0,syms['__flash_code_end'])
    addr=lambda n:syms[n]&~1
    # Original scatter loader walks records until r4 == r6; application_main
    # and its pre-board system-init call preserve r4. Read the table end
    # from the actual original literal at 0x334, rather than inventing r4.
    scatter_end=struct.unpack_from("<I",backup,0x334)[0]
    rng=random.Random(0x42c2026);used=set();count=0;writes=0;raw_diffs=set()
    for clock in (4000000,8000000,16000000,24000000,32000000,48000000):
        for trial in range(12):
            ram=bytes(8192) if trial==0 else rng.getrandbits(8192*8).to_bytes(8192,'little')
            mmio={a:rng.getrandbits(32) for a in sorted(used)} if trial>=2 else {}
            # Buttons released: factory intentionally halts if PA9 is low.
            mmio[0x40020d04]=mmio.get(0x40020d04,0)|(1<<9)
            regs=[rng.getrandbits(32) for _ in range(13)]
            vms=[];events=[[],[]]
            for j,legacy in enumerate((True,False)):
                def frequency(v,j=j):events[j].append(('clock',clock));v.r[0]=clock
                def delay(v,j=j):events[j].append(('delay_ms',v.r[0]))
                boundaries={'peripheral_clock_frequency':(0x1924,frequency),'factory_delay_ms':(0x2de8,delay)}
                v=BoardVM(backup if legacy else flash,oi if legacy else ni,ram,mmio,legacy=legacy,
                    points={p if legacy else addr(n):n for n,p in POINTS.items()},
                    callbacks={(p if legacy else addr(n)):(n,f) for n,(p,f) in boundaries.items()})
                v.r[:13]=regs;v.r[4]=scatter_end;v.execute(0x2608 if legacy else addr('factory_board_init'))
                vms.append(v)
            a,b=vms;used.update(a.mmio);used.update(b.mmio)
            assert a.trace==b.trace,(clock,trial,'register write trace differs',next(((i,x,y) for i,(x,y) in enumerate(zip(a.trace,b.trace)) if x!=y),None),len(a.trace),len(b.trace),a.pins,b.pins,a.entries,b.entries)
            assert a.mmio==b.mmio and a.ram[:0xa54]==b.ram[:0xa54] and events[0]==events[1],(clock,trial,'state or call mismatch')
            assert pin_effects(a.pins)==pin_effects(b.pins),(clock,trial,'effective GPIO config mismatch',a.pins,b.pins)
            assert dict(a.entries)['initialize_indicator']==dict(a.entries)['initialize_encoder_bus']==1
            for x,y in zip(a.pins,b.pins):
                if x!=y:raw_diffs.add((x,y))
            count+=1;writes+=len(a.trace)
    print(f'PASS: {count} complete board-init scenarios; {writes} ordered peripheral/NVIC register writes matched; RAM and final register state matched. Original caller r4 derived from scatter loader; clock and delay scripted, hardware/interrupts not modeled.',flush=True)
    print('Original raw GPIO configuration differences with identical register effects:',sorted(raw_diffs),flush=True)

class ResetVM(BoardVM):
    def read(self,a,n):
        # Synthetic ROM trim values, PLL ready and elapsed SysTick countdown.
        if 0x100c00<=a<=0x100c30-n:
            return int.from_bytes(self.trim[a-0x100c00:a-0x100c00+n],'little')
        if a==0x4000203c and n==4:return super().read(a,n)|0x40000
        if a==0xe000e018 and n==4:return 0
        return super().read(a,n)
    def step(self,p):
        if p==self.board_entry:self.board_r4=self.r[4]
        return super().step(p)

def compare_reset_to_identity(elf,flash,syms,oldelf,backup):
    import json,pathlib
    from integration import Checkpoint
    oi=load_ins(oldelf,0xc0,0x7a00);ni=load_ins(elf,0xc0,syms['__flash_code_end']);addr=lambda n:syms[n]&~1
    meta=json.loads((pathlib.Path(__file__).resolve().parents[1]/'factory/tests/layout_expected.json').read_text())
    rng=random.Random(0x42c314);writes=0;scatter_end=struct.unpack_from('<I',backup,0x334)[0]
    def stop(v):raise Checkpoint
    for k in range(24):
        ram=rng.getrandbits(8192*8).to_bytes(8192,'little');regs=[rng.getrandbits(32) for _ in range(13)]
        trim=bytearray(48)
        base=0x100+8*k;struct.pack_into('<5H',trim,0,*range(base,base+5));struct.pack_into('<2H',trim,32,base+5,base+6)
        mmio={0x40020d04:1<<9};vms=[]
        for legacy in (True,False):
            v=ResetVM(backup if legacy else flash,oi if legacy else ni,ram,mmio,legacy=legacy,
                points={p if legacy else addr(n):n for n,p in POINTS.items()},
                callbacks={(0x2c2c if legacy else addr('factory_identity_check')):('identity',stop)})
            v.trim=trim;v.board_entry=0x2608 if legacy else addr('factory_board_init');v.r[:13]=regs
            try:v.execute(struct.unpack_from('<I',backup if legacy else flash,4)[0]&~1,limit=200000)
            except Checkpoint:pass
            else:raise AssertionError('identity checkpoint not reached')
            vms.append(v)
        a,b=vms
        assert a.board_r4==scatter_end,(k,a.board_r4,scatter_end)
        assert a.trace==b.trace and a.mmio==b.mmio and pin_effects(a.pins)==pin_effects(b.pins),(k,'reset peripheral mismatch')
        expected=bytearray(a.ram[:0xa54])
        for row in meta['callbacks']:struct.pack_into('<I',expected,row['offset']+12,syms[row['symbol']]|1)
        assert expected==b.ram[:0xa54],(k,'reset workspace mismatch',[(hex(i),x,y) for i,(x,y) in enumerate(zip(expected,b.ram)) if x!=y][:12])
        assert a.calls==b.calls==['identity'];writes+=len(a.trace)
    print(f'PASS: 24 actual reset-vector-to-identity-check executions; {writes} ordered register writes matched; scatter-loaded workspace matched after callback relocation. Original board entry r4=0x{scatter_end:04X}. Synthetic ROM trim, PLL-ready and SysTick reads; interrupts and electrical timing not modeled.',flush=True)
