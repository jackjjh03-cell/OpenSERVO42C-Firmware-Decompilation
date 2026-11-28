#!/usr/bin/env python3
"""Export an offline, unvalidated board-specific image from an IDE-built ELF."""
import argparse, hashlib, json, pathlib, sys, tempfile, struct
from inspect_elf import read_application, LIMIT
ROOT=pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'factory'/'tests'))
from check_backup import read_backup
def validate_vectors(b):
    sp,reset=struct.unpack_from("<II",b)
    if not (0x20000000<sp<=0x20002000 and sp%4==0 and reset&1 and 0xc0<=reset<0x7a00):
        raise ValueError("Unexpected initial stack or reset vector")
# Known application of the single V1.1.3 capture used for reconstruction.
# Reject unknown revisions instead of assuming their reserved layout is identical.
FACTORY_APP_SHA='1a64209c9cb46679f78628b8eb82cddcb5ef78972b9f6699a9b41aeb12610595'
def sha(b):return hashlib.sha256(b).hexdigest()
def check_factory(b):
 validate_vectors(b)
 if sha(b[:LIMIT])!=FACTORY_APP_SHA:
  raise ValueError('Backup application does not match the reconstructed factory revision; no candidate generated.')

def encode_hex(data):
 if len(data)!=65536:raise ValueError('Full 64 KiB required.')
 def record(addr,kind,chunk):
  payload=bytes([len(chunk)])+addr.to_bytes(2,'big')+bytes([kind])+chunk
  return ':'+(payload+bytes([-sum(payload)&255])).hex().upper()
 return '\n'.join([record(i,0,data[i:i+16]) for i in range(0,len(data),16)]+[record(0,1,b'')])+'\n'



def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('elf',type=pathlib.Path);parser.add_argument('backup',type=pathlib.Path);parser.add_argument('output',type=pathlib.Path)
    parser.add_argument('--acknowledge-unvalidated',action='store_true');args=parser.parse_args()
    if not args.acknowledge_unvalidated: parser.error('This is not hardware validated; explicit acknowledgment required.')
    if not __debug__:parser.error('Do not use Python -O.')
    if args.output.exists():parser.error('Choose a new output directory.')
    app,end=read_application(args.elf)
    report=json.loads((args.elf.parent/'build_report.json').read_text())
    if sha(app)!=report['application_sha256']:raise ValueError('ELF differs from IDE build report')
    factory=read_backup(args.backup);check_factory(factory)
    full=app+factory[LIMIT:]
    encoded=encode_hex(full)
    with tempfile.TemporaryDirectory() as tmp:
        path=pathlib.Path(tmp)/'roundtrip.hex';path.write_text(encoded,encoding='ascii')
        if read_backup(path)!=full or len(full)!=65536:raise ValueError('HEX round-trip failed')
    args.output.mkdir(parents=True,exist_ok=False)
    (args.output/'UNVALIDATED_candidate.hex').write_text(encoded,encoding='ascii')
    (args.output/'UNVALIDATED_candidate.bin').write_bytes(full)
    (args.output/'manifest.json').write_text(json.dumps({'status':'UNVALIDATED OFFLINE CANDIDATE; NOT APPROVED FOR FLASHING','application_sha256':sha(app),'backup_sha256':sha(factory),'candidate_sha256':sha(full),'application_extent':end,'reserved_bytes_preserved':full[LIMIT:]==factory[LIMIT:]},indent=2)+'\n')
    print('Offline output only; all 0x7A00–0xFFFF bytes preserved from supplied same-board backup. Not hardware validated.')
if __name__=='__main__':main()
