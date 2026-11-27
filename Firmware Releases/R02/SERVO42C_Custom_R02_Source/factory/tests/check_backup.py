#!/usr/bin/env python3
"""Offline validation of two independently read main-flash backups.
No serial connection, programming, unlocking or device modifications.
"""
import argparse, hashlib, pathlib, struct

def read_backup(p):
 p=pathlib.Path(p);raw=p.read_bytes()
 if p.suffix.lower()=='.bin':
  if len(raw)!=65536:raise ValueError(f'{p}: expected exactly 65536 bytes')
  return raw
 if p.suffix.lower()!='.hex':raise ValueError('Expected .hex or .bin')
 memory=bytearray(65536);seen=set();base=0;eof=False
 for i,line in enumerate(raw.decode('ascii').splitlines(),1):
  if not line.strip():continue
  if eof:raise ValueError(f'{p}:{i}: record after EOF')
  if not line.startswith(':'):raise ValueError(f'{p}:{i}: missing record prefix')
  record=bytes.fromhex(line[1:])
  if len(record)<5 or len(record)!=record[0]+5 or sum(record)&255:raise ValueError(f'{p}:{i}: invalid length/checksum')
  count,address,typ=record[0],int.from_bytes(record[1:3],'big'),record[3];data=record[4:-1]
  if typ==0:
   for j,b in enumerate(data):
    a=base+address+j
    if a>=65536 or a in seen:raise ValueError(f'{p}:{i}: duplicate or out-of-range address {a:#x}')
    seen.add(a);memory[a]=b
  elif typ==1:
   if count or address:raise ValueError('Invalid EOF')
   eof=True
  elif typ in (2,4):
   if count!=2 or address:raise ValueError('Invalid extended address')
   base=int.from_bytes(data,'big')<<(4 if typ==2 else 16)
  elif typ in (3,5):
   if count!=4 or address:raise ValueError('Invalid start address record')
  else:raise ValueError(f'Unknown record type {typ}')
 if not eof or len(seen)!=65536:raise ValueError(f'{p}: incomplete main flash: {len(seen)} of 65536 addresses')
 return bytes(memory)

def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('backup');parser.add_argument('second_backup',nargs='?');args=parser.parse_args()
 try:
  b=read_backup(args.backup);sp,reset=struct.unpack_from('<II',b)
  if not (0x20000000<sp<=0x20002000 and sp%4==0 and reset&1 and 0xc0<=reset<0x7a00):raise ValueError('Unexpected initial stack or reset vector')
  if args.second_backup:
   other=read_backup(args.second_backup)
   if b!=other:raise ValueError(f'Backups differ at {sum(a!=c for a,c in zip(b,other))} bytes')
   print('Both files contain identical main-flash bytes. Independence of the two reads must be established by the operator.')
  else:print('Only one backup supplied; no repeat-read comparison performed.')
  for name,a,z in [('Entire main flash',0,0x10000),('Application',0,0x7a00),('Identity',0x7a00,0x7c00),('Settings',0x7c00,0x7e00),('Calibration knots',0x7e00,0x8000),('Encoder LUT',0x8000,0x10000)]:
   print(f'{name:19} {a:04X}-{z-1:04X} SHA256 {hashlib.sha256(b[a:z]).hexdigest()}')
  print('PASS: full 64 KiB backup structure. Not a flash authorization; do not share device-specific contents.')
 except (ValueError,OSError) as exc:parser.exit(1,f'FAIL: {exc}\n')
if __name__=='__main__':main()
