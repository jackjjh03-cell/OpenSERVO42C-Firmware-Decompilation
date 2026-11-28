"""Extract only file-backed application bytes from an ARM ELF32 image."""
import struct

FLASH_END = 0x7600
STACK_TOP = 0x20002000


def application_bytes(elf):
    if len(elf)<52 or elf[:7]!=b'\x7fELF\x01\x01\x01':
        raise ValueError('Expected little-endian ELF32')
    kind,machine,version,entry,phoff,_,_,ehsize,phsize,phnum,*_ = struct.unpack_from('<HHIIIIIHHHHHH',elf,16)
    if (kind,machine,version,ehsize,phsize)!=(2,40,1,52,32) or not phnum or phoff+phsize*phnum>len(elf):
        raise ValueError('Invalid ARM executable program headers')
    data={}
    for i in range(phnum):
        typ,offset,virt,phys,size,memsize,flags,align=struct.unpack_from('<8I',elf,phoff+i*phsize)
        if typ!=1 or not size:
            continue  # not a loadable region, or RAM-only zero initialization
        if size>memsize or offset+size>len(elf) or phys+size>FLASH_END:
            raise ValueError('File-backed region outside application flash')
        for j,v in enumerate(elf[offset:offset+size],phys):
            if j in data:
                raise ValueError('Overlapping loadable regions')
            data[j]=v
    if not all(i in data for i in range(192)):
        raise ValueError('Missing vector table')
    stack,reset=struct.unpack('<II',bytes(data[i] for i in range(8)))
    if stack!=STACK_TOP or reset!=entry or not reset&1 or (reset&~1) not in data:
        raise ValueError('Invalid reset vector or stack')
    return data


def record(addr,kind,payload):
    raw=bytes((len(payload),addr>>8,addr&255,kind))+payload
    return ':'+(raw+bytes((-sum(raw)&255,))).hex().upper()


def intel_hex(data):
    if not data or min(data)<0 or max(data)>=FLASH_END:
        raise ValueError('Invalid application address range')
    addresses=sorted(data);lines=[];i=0
    while i<len(addresses):
        addr=addresses[i];payload=bytearray()
        while i<len(addresses) and len(payload)<16 and addresses[i]==addr+len(payload):
            payload.append(data[addresses[i]]);i+=1
        lines.append(record(addr,0,bytes(payload)))
    lines.append(record(0,1,b''))
    return '\n'.join(lines)+'\n'
