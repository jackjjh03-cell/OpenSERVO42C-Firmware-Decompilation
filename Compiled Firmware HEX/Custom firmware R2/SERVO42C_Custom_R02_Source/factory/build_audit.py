#!/usr/bin/env python3
"""Source-only physical-layout link audit. No firmware image is saved or flashed."""
import pathlib, subprocess, tempfile, shutil, struct, json, re, sys, hashlib
ROOT=pathlib.Path(__file__).resolve().parent
CLANG=shutil.which('clang'); LD=shutil.which('ld.lld'); OBJDUMP=shutil.which('llvm-objdump'); NM=shutil.which('llvm-nm') or shutil.which('nm')
if not all((CLANG,LD,OBJDUMP,NM)):raise SystemExit('Requires clang, ld.lld, llvm-objdump and nm on PATH')
flags=['--target=arm-none-eabi','-mcpu=cortex-m0plus','-mthumb','-Oz','-fomit-frame-pointer','-ffunction-sections','-fdata-sections','-ffreestanding','-fno-builtin','-fno-strict-aliasing','-Wall','-Wextra','-Werror','-I'+str(ROOT/'include')]
def run(args):return subprocess.check_output(args,text=True)
def main():
 with tempfile.TemporaryDirectory() as td:
  d=pathlib.Path(td); objs=[]
  edit="--edit-smoke" in sys.argv
  if edit and "--compare-factory" in sys.argv:raise SystemExit("Run edit-smoke and compare-factory separately")
  for original_source in sorted((ROOT/'src').glob('*.c')):
   source=original_source
   if edit and source.name in ('reset.c','layout_startup_data.c'):
    text=source.read_text()
    if source.name=='reset.c':
     text,n=re.subn(r'void reset_handler\(void\)\s*\{','volatile uint32_t edit_counter=0x12345678u;\\nvolatile uint32_t edit_zero;\\nvoid reset_handler(void) {',text,count=1);assert n==1
     text=text.replace('initialize_c_globals();','initialize_c_globals();edit_zero=edit_counter;')
    else:text=text.replace('"UartBaud"','"Serial"')
    source=d/source.name;source.write_text(text)
   obj=d/(source.stem+'.o');subprocess.run([CLANG,*flags,'-c',str(source),'-o',str(obj)],check=True);objs.append(obj)
  elf=d/'audit.elf';subprocess.run([LD,'--gc-sections','--icf=safe','--no-undefined','-T',str(ROOT/'layout.ld'),'-Map='+str(d/'audit.map'),'-o',str(elf),*map(str,objs)],check=True)
  assert not run([NM,'-u',str(elf)]).strip()
  syms={line.split()[-1]:int(line.split()[0],16) for line in run([NM,'-n',str(elf)]).splitlines() if len(line.split())>=3 and re.fullmatch(r'[0-9a-fA-F]+',line.split()[0])}
  print(f'{len(objs)} C translation units compiled; no unresolved symbols.')
  print(run([OBJDUMP,'-h',str(elf)]))
  data=elf.read_bytes(); header=struct.unpack_from('<16sHHIIIIIHHHHHH',data);shoff,shentsize,shnum,shstr=header[6],header[11],header[12],header[13]
  sh=[struct.unpack_from('<10I',data,shoff+i*shentsize) for i in range(shnum)];strings=data[sh[shstr][4]:sh[shstr][4]+sh[shstr][5]]
  sections={strings[x[0]:].split(b'\0',1)[0].decode():x for x in sh};v=sections['.vectors'];words=struct.unpack_from('<48I',data,v[4]);assert words[0]==0x200015f8 and words[1]==(syms['reset_handler']|1)
  assert words[2]==(syms['default_handler']|1) and words[3]==(syms['hardfault_handler']|1)
  assert words[21]==(syms['motion_timer_irq']|1) and words[22]==(syms['uart0_irq']|1) and words[32]==(syms['auxiliary_timer_irq']|1)
  assert all((x==0 or (x&1 and 0xc0<=x<0x7a00)) for x in words[1:])
  assert sum(w!=0 for w in words[1:])==38
  assert syms['work_ram']==0x20000000 and syms['__stack_top']==0x200015f8
  assert syms['__app_flash_end']<=0x7a00
  print(f'Vector table: 48 words; reset=0x{words[1]:08x}; stack=0x{words[0]:08x}.')
  print(f'Application ends at 0x{syms["__app_flash_end"]:04x}; remaining before identity page: {0x7a00-syms["__app_flash_end"]} bytes.')
  # Verify every load segment that contributes bytes to physical application flash.
  phoff,phentsize,phnum=header[5],header[9],header[10]
  for j in range(phnum):
   typ,off,vaddr,paddr,filesz,memsz,pflags,align=struct.unpack_from('<8I',data,phoff+j*phentsize)
   if typ==1 and filesz:
    assert paddr<0x7a00 and paddr+filesz<=0x7a00,('load segment overlaps reserved flash',hex(paddr),filesz)
  image=bytearray(b'\xff'*0x7a00)
  for j in range(phnum):
   typ,off,vaddr,paddr,filesz,memsz,pflags,align=struct.unpack_from('<8I',data,phoff+j*phentsize)
   if typ==1 and filesz:image[paddr:paddr+filesz]=data[off:off+filesz]
  print('Application SHA-256 (including erased padding to 0x7A00): '+hashlib.sha256(image).hexdigest())
  if edit:
   ds,bs=sections['.data'],sections['.bss'];assert ds[5]>=4 and bs[5]>=4
   assert 0x20001600<=syms['edit_counter']<0x20002000 and 0x20001600<=syms['edit_zero']<0x20002000
   assert data[ds[4]:ds[4]+4]==bytes.fromhex('78563412')
   assert b'Serial\0' in data and b'UartBaud\0' not in data
   print('PASS: source edit renamed a menu entry and added initialized/zero-initialized globals; new link fits reserved layout.')
  if "--compare-factory" in sys.argv:
   sys.path.insert(0,str(ROOT/"tests"));from compare_layout import check
   flash=bytearray(syms["__flash_code_end"])
   for name in (".vectors",".text",".rodata",".ARM.exidx",".ARM.extab"):
    if name in sections:
     x=sections[name];flash[x[3]:x[3]+x[5]]=data[x[4]:x[4]+x[5]]
   check(elf,bytes(flash),syms,words)
  if any(x in sys.argv for x in ('--integration','--persistence','--defaults-test')):
   sys.path.insert(0,str(ROOT/'tests'));from integration import check_integration
   flash=bytearray(b'\xff'*0x10000)
   for j in range(phnum):
    typ,off,vaddr,paddr,filesz,memsz,pflags,align=struct.unpack_from('<8I',data,phoff+j*phentsize)
    if typ==1 and filesz:flash[paddr:paddr+filesz]=data[off:off+filesz]
   if '--integration' in sys.argv:check_integration(elf,bytes(flash),syms,words,edit)
   if '--persistence' in sys.argv:
    from persistence import check_persistence
    check_persistence(elf,bytes(flash),syms)
   if '--defaults-test' in sys.argv:
    from persistence import check_default_edit
    check_default_edit(elf,bytes(flash),syms)
  print('PASS: independent physical-address link with source assets, reserved pages and vector checks. No factory binary input; no output ELF retained. NOT hardware tested or approved for flashing.')
if __name__=='__main__':main()
