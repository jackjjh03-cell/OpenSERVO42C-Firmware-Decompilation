#!/usr/bin/env python3
"""Compare the actual IDE-built factory ELF to an original private 64 KiB dump.
Uses a limited Thumb interpreter and modeled hardware. No programming/flash I/O.
"""
import argparse, hashlib, pathlib, re, shutil, struct, subprocess, sys, tempfile
from inspect_elf import read_application
from inspect_settings import decode
ROOT=pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'factory'/'tests'))
import integration, protocol_compat, startup_compat
from integration import VM,put
from thumb_model import load_ins
from check_backup import read_backup

def nm(path):
    tool=shutil.which('llvm-nm') or shutil.which('nm')
    if not tool:raise RuntimeError('nm required')
    return {l.split()[-1]:int(l.split()[0],16) for l in subprocess.check_output([tool,'-n',str(path)],text=True).splitlines() if len(l.split())>=3 and re.fullmatch('[0-9a-fA-F]+',l.split()[0])}

def wrap_original(backup, dest):
    # Explicit instructions in an ELF wrapper are for decoding only. Preserve
    # raw bytes including literals. Function entry labels prevent decoding a
    # literal immediately preceding a function as half a Thumb-2 instruction.
    import csv
    entries={int(r['address'],16) for r in csv.DictReader((ROOT/'docs'/'original_function_inventory.csv').open())}
    asm=dest/'original.s'; obj=dest/'original.o'; elf=dest/'firmware.elf'
    out=['.syntax unified','.thumb','.section .text,"ax",%progbits']
    for a,w in enumerate(struct.unpack('<32768H',backup)):
        if 2*a in entries:out.extend(['.thumb_func',f'original_{2*a:04x}:'])
        out.append(f'.inst.n 0x{w:04x}')
    asm.write_text('\n'.join(out)+'\n')
    subprocess.run(['clang','--target=arm-none-eabi','-mcpu=cortex-m0plus','-mthumb','-c',str(asm),'-o',str(obj)],check=True)
    subprocess.run(['ld.lld','-Ttext=0','--entry=0','-o',str(elf),str(obj)],check=True)
    data=elf.read_bytes(); h=struct.unpack_from('<16sHHIIIIIHHHHHH',data)
    assert any(typ==1 and pa==0 and fs==65536 and data[off:off+fs]==backup for typ,off,va,pa,fs,ms,fl,al in (struct.unpack_from('<8I',data,h[5]+i*h[9]) for i in range(h[10])))
    return elf

def settings_replay(elf,flash,syms,oldelf,backup):
    oi=load_ins(oldelf,0xc0,0x7a00);ni=load_ins(elf,0xc0,syms['__flash_code_end']); addr=lambda n:syms[n]&~1
    # Replay each historical state and the actual full page; also compare a
    # blank page and invalid latest marker using original selection semantics.
    pages=[]
    for n in range(4):pages.append((f'{n} saved slots',backup[0x7c00:0x7c00+64*n]+b'\xff'*(512-64*n)))
    pages.append(('exact full dump',backup[0x7c00:0x7e00]))
    invalid=bytearray(pages[-1][1]);invalid[128+54:128+56]=b'\xff\xff';pages.append(('invalid latest marker',bytes(invalid)))
    events_total=0
    for label,page in pages:
        of=bytearray(backup);of[0x7c00:0x7e00]=page
        nf=bytearray(flash);nf[0x7c00:0x7e00]=page
        expect=decode(of);old=VM(bytes(of),oi,bytearray(8192),{},legacy=True);new=VM(bytes(nf),ni,bytearray(8192),{})
        old.execute(0x5368);new.execute(addr('load_or_initialize_settings'))
        assert old.ram[:0xa54]==new.ram[:0xa54],(label,'settings RAM mismatch')
        r=new.ram; active=int.from_bytes(r[4:8],'little');baud=int.from_bytes(r[0x73c:0x740],'little');mode=r[0x735];dev=int.from_bytes(r[0x740:0x742],'little');kp=int.from_bytes(r[0x5ea:0x5ec],'little')
        assert active==int(expect['factory_selected_address'],16)
        if expect['uses_saved_record']:
            row=expect['records'][expect['factory_selected_slot']];assert (baud,mode,dev,kp)==(row['baud'],row['mode_raw'],row['uart_address'],row['kp_raw'])
        else:assert (baud,mode,dev,kp)==(0,1,0xe0,1638)
        # Exercise original UART initialization using identical modeled clock.
        # Configuration and NVIC side effects stubbed at explicit boundaries.
        events=[[],[]]
        oldnames={'peripheral_clock_frequency':0,'uart_configure':0x21f0,'factory_irq_config':0xfec}
        oldnames['peripheral_clock_frequency']=0x1924 # original BL at 0x5BF8
        for j,v in enumerate((old,new)):
            def clock(vm,j=j):events[j].append(('clock',));vm.r[0]=24000000
            def cfg(vm,j=j):events[j].append(('uart_config',vm.r[0],tuple(vm.read(vm.r[1]+4*k,4) for k in range(4))));vm.r[0]=0
            def irq(vm,j=j):events[j].append(('irq',tuple(vm.r[:3])))
            cb={'peripheral_clock_frequency':clock,'uart_configure':cfg,'factory_irq_config':irq}
            v.callbacks={(oldnames[n] if j==0 else addr(n)):(n,fn) for n,fn in cb.items()}
            v.execute(0x5bd4 if j==0 else addr('factory_apply_uart'))
        assert old.ram[:0xa54]==new.ram[:0xa54] and old.mmio==new.mmio and events[0]==events[1],(label,events,old.mmio,new.mmio)
        if baud:assert events[0][1]==('uart_config',0x40000000,(64,512,24000000,baud))
        else:assert not events[0]
        events_total+=1
        print(f'PASS {label}: slot=0x{active:04X} mode={mode} baud={baud} address=0x{dev:02X} kp_raw={kp}; settings RAM and UART apply match factory',flush=True)
    print(f'PASS: {events_total} settings-history/load and UART configuration scenarios; actual dump plus controlled variants.',flush=True)

def settings_write_replay(elf,flash,syms,oldelf,backup):
    from persistence import FlashVM
    oi=load_ins(oldelf,0xc0,0x7a00);ni=load_ins(elf,0xc0,syms['__flash_code_end']);addr=lambda n:syms[n]&~1
    # Isolate just the user's two menu changes, starting from the earliest
    # stored record. Real commit/flash code runs; motor transitions and
    # UART reconfiguration are deliberately mocked for this offline test.
    page=backup[0x7c00:0x7c40]+b'\xff'*448
    f0=bytearray(backup);f0[0x7c00:0x7e00]=page
    f1=bytearray(flash);f1[0x7c00:0x7e00]=page
    boundaries={'factory_pause_control':0x5874,'factory_resume_control':0x58a0,'factory_apply_mode':0x570c,'factory_apply_direction':0x583c,'factory_apply_uart':0x5bd4,'factory_refresh_mode':0x2090}
    def no_op(v):pass
    def mode(v):v.r[0]=0
    def callbacks(legacy):return {(p if legacy else addr(name)):(name,mode if name=='factory_apply_mode' else no_op) for name,p in boundaries.items()}
    old=FlashVM(f0,oi,bytearray(8192),{0x4000580c:0},legacy=True,callbacks=callbacks(True))
    new=FlashVM(f1,ni,bytearray(8192),{0x4000580c:0},callbacks=callbacks(False))
    old.execute(0x5368);new.execute(addr('load_or_initialize_settings'))
    for entry,name,option,expected_slot in [(0x2d34,'menu_control_mode',2,1),(0x4084,'menu_uart_baud',6,2)]:
        old.r[0]=option;new.r[0]=option
        old.execute(entry,limit=200000);new.execute(addr(name),limit=200000)
        assert old.ram[:0xa54]==new.ram[:0xa54] and old.mmio==new.mmio and old.operations==new.operations and old.calls==new.calls,(name,'commit mismatch',old.calls,new.calls,[(hex(i),x,y) for i,(x,y) in enumerate(zip(old.ram[:0xa54],new.ram[:0xa54])) if x!=y][:20])
        assert old.flash[0x7a00:]==new.flash[0x7a00:]
        active=decode(bytes(new.flash));assert active['factory_selected_slot']==expected_slot
        row=active['records'][expected_slot];a=0x7c00+64*expected_slot
        actual=backup[a:a+64];modeled=bytes(new.flash[a:a+64]);diffs=[(hex(a+i),f'{x:02X}',f'{y:02X}') for i,(x,y) in enumerate(zip(actual,modeled)) if x!=y]
        print(f'PASS {name}({option}): original/reconstructed menu and flash writes match; captured-vs-ideal slot byte differences {diffs}',flush=True)
    print('PASS: two actual menu changes replayed through original and reconstructed machine code with idealized NOR storage; physical flash faults and unobserved history are not modeled.',flush=True)

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('elf',type=pathlib.Path);p.add_argument('backup',type=pathlib.Path);p.add_argument('--full',action='store_true',help='also run 790 integration scenarios and expanded protocol/startup comparisons');p.add_argument('--extra',action='store_true',help='compare relocated telemetry');p.add_argument('--board',action='store_true',help='compare complete board initialization');p.add_argument('--suite',choices=['integration','protocol','startup'],help='run just one large regression group, in addition to settings');a=p.parse_args()
    backup=read_backup(a.backup);app,end=read_application(a.elf);syms=nm(a.elf)
    if hashlib.sha256(backup[:0x7a00]).hexdigest()!="1a64209c9cb46679f78628b8eb82cddcb5ef78972b9f6699a9b41aeb12610595":p.error('Backup application does not match the V1.1.3 instruction reference')
    # Overlay unchanged private reserved data for comparisons only.
    flash=app+backup[0x7a00:];print('Factory dump SHA256:',hashlib.sha256(backup).hexdigest(),flush=True);print('Candidate application SHA256:',hashlib.sha256(app).hexdigest(),flush=True)
    with tempfile.TemporaryDirectory() as t:
        d=pathlib.Path(t);(d/'build').mkdir();(d/'reference').mkdir();(d/'reference'/'factory.bin').write_bytes(backup)
        # Tests expect private fixtures beside their source root; use isolated paths.
        oldelf=wrap_original(backup,d/'build')
        settings_replay(a.elf,flash,syms,oldelf,backup)
        settings_write_replay(a.elf,flash,syms,oldelf,backup)
        if a.extra:
            from extra_factory_checks import compare_extra
            compare_extra(a.elf,flash,syms,oldelf,backup)
        if a.board or a.full:
            from board_init_compare import compare_board_init, compare_reset_to_identity
            compare_board_init(a.elf,flash,syms,oldelf,backup)
            compare_reset_to_identity(a.elf,flash,syms,oldelf,backup)
        if a.full or a.suite:
            class FixtureRoot:
                parent=d
                def __truediv__(self,p):return ROOT/'factory'/p
            for module in (integration,protocol_compat,startup_compat):module.ROOT=FixtureRoot()
            words=struct.unpack_from('<48I',app)
            if a.full or a.suite=='integration':integration.check_integration(a.elf,flash,syms,words)
            if a.full or a.suite=='protocol':protocol_compat.check_protocol(a.elf,flash,syms)
            if a.full or a.suite=='startup':startup_compat.check_startup(a.elf,flash,syms)
    print('PASS: requested software comparison checks. Hardware timing and behavior remain unverified.',flush=True)
if __name__=='__main__':main()
