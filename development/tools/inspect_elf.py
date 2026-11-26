#!/usr/bin/env python3
"""Inspect a linked application offline. Does not emit flashable images."""
import argparse, hashlib, json, pathlib, struct
LIMIT=0x7a00

def read_application(path):
    data=pathlib.Path(path).read_bytes()
    h=struct.unpack_from('<16sHHIIIIIHHHHHH',data)
    if h[0][:6]!=b'\x7fELF\x01\x01' or h[1]!=2 or h[2]!=40 or h[9]!=32:
        raise ValueError('Expected linked ELF32 little-endian ARM executable')
    app=bytearray(b'\xff'*LIMIT); used=set()
    for j in range(h[10]):
        typ,off,vaddr,paddr,filesz,memsz,flags,align=struct.unpack_from('<8I',data,h[5]+j*h[9])
        if typ!=1 or not filesz: continue
        if paddr+filesz>LIMIT or off+filesz>len(data): raise ValueError('Invalid application load segment')
        span=set(range(paddr,paddr+filesz))
        if span&used: raise ValueError('Overlapping load segments')
        used.update(span);app[paddr:paddr+filesz]=data[off:off+filesz]
    if not set(range(192))<=used: raise ValueError('Missing vectors')
    vectors=struct.unpack_from('<48I',app)
    if vectors[0]!=0x200015f8 or not vectors[1] or any(v and (not v&1 or v&~1 not in used) for v in vectors[1:]):
        raise ValueError('Invalid vector table')
    return bytes(app), max(used)+1

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('elf');p.add_argument('--report',type=pathlib.Path);p.add_argument('--factory',type=pathlib.Path);a=p.parse_args()
    app,end=read_application(a.elf)
    info={'status':'Unvalidated development build; not hardware tested','application_sha256':hashlib.sha256(app).hexdigest(),'loaded_extent_bytes':end,'free_before_identity':LIMIT-end}
    if a.factory:
        import sys
        sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]/'factory'/'tests'))
        from check_backup import read_backup
        factory=read_backup(a.factory)[:LIMIT]
        info.update(factory_application_sha256=hashlib.sha256(factory).hexdigest(),differing_bytes_at_same_addresses=sum(x!=y for x,y in zip(app,factory)),compared_bytes=LIMIT)
    print(json.dumps(info,indent=2))
    if a.report:a.report.write_text(json.dumps(info,indent=2)+'\n')
if __name__=='__main__':main()
