"""Link all firmware code with startup/runtime into a non-deployable test ELF."""
import os
import argparse
from pathlib import Path
import subprocess
import tempfile
ROOT=Path(__file__).resolve().parents[1]
FW=ROOT/'firmware'

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument("--bringup", action="store_true", help="Link the hardware acquisition application instead of the fixture")
    parser.add_argument("--motor", action="store_true", help="Link the motor-runtime variant; not deployable")
    parser.add_argument("--optimized", action="store_true", help="Use LTO and section garbage collection for the selected application")
    parser.add_argument("--gc-only", action="store_true", help="Use section garbage collection without LTO")
    args=parser.parse_args()
    if args.motor and args.bringup:
        parser.error("choose acquisition or motor runtime")
    application=args.bringup or args.motor
    if (args.optimized or args.gc_only) and not application:
        parser.error("optimized links require --bringup or --motor; retain all modules in the fixture")
    if args.optimized and args.gc_only:
        parser.error("choose one optimization mode")
    with tempfile.TemporaryDirectory() as tmp:
        out=Path(tmp)
        files=sorted(p for group in ('core','hal','motion','communication','target') for p in (FW/group).glob('*') if p.suffix in ('.c','.S'))
        files.append(FW/'application'/'bringup_main.c' if application else ROOT/'tests'/'target_link_probe.c')
        objects=[]
        for index,src in enumerate(files):
            obj=out/('module_%d.o'%index)
            flags=['-Wall','-Wextra','-Werror','-Wconversion','-Wsign-conversion','-ffreestanding','-fno-builtin','-ffunction-sections','-fdata-sections'] if src.suffix=='.c' else []
            if args.motor and src.name=='bringup_main.c':
                flags.append('-DOS_MOTOR_RUNTIME=1')
            if args.optimized and src.suffix=='.c' and src.name!='runtime.c':
                flags.append('-flto')
            subprocess.run([os.getenv('ARM_CLANG','clang'),'--target=arm-none-eabi','-mcpu=cortex-m0plus','-mthumb','-Oz',*flags,'-I',str(FW/'include'),'-c',str(src),'-o',str(obj)],check=True)
            objects.append(str(obj))
        elf=out/('MOTOR_LINK_TEST.elf' if args.motor else 'BRINGUP_LINK_TEST.elf' if args.bringup else 'LINK_TEST_ONLY.elf')
        link_flags=['--gc-sections'] if args.optimized or args.gc_only else []
        if args.optimized:
            link_flags.append('--lto-O2')
        subprocess.run([os.getenv('ARM_LLD','ld.lld'),'-T',str(FW/'target'/'hc32l130.ld'),'--no-undefined',*link_flags,'-Map='+str(out/'probe.map'),*objects,'-o',str(elf)],check=True)
        nm=os.getenv('ARM_NM','nm')
        assert not subprocess.check_output([nm,'-u',str(elf)],text=True).strip()
        symbols={line.split()[-1]:int(line.split()[0],16) for line in subprocess.check_output([nm,str(elf)],text=True).splitlines() if len(line.split())==3}
        for name in ('vectors','reset_handler','os_target_main','__app_flash_end','__static_bytes','__settings_slot0','__settings_slot1'):
            assert name in symbols, name
        assert symbols['vectors']==0 and symbols['__settings_slot0']==0x7600 and symbols['__settings_slot1']==0x7800
        flash=symbols['__app_flash_end'];ram=symbols['__static_bytes']
        assert flash<=0x7000 and ram<=0x1800 and symbols['__stack_top']==0x20002000
        print('{} [{}]: {} bytes flash, {} bytes static RAM, {} bytes reserved stack; no unresolved symbols'.format('Motor runtime application' if args.motor else 'Board bring-up application' if args.bringup else 'Target link fixture','LTO + GC' if args.optimized else 'GC' if args.gc_only else 'all modules',flash,ram,symbols['__stack_top']-symbols['__stack_floor']))
        print('Link check only; no HEX or deployable artifact produced.')
if __name__=='__main__':main()
