"""Compare relocated telemetry output against original instructions.
Physical display and original 64-bit division are modeled explicitly.
"""
import random
from integration import VM,put
from thumb_model import load_ins

def compare_extra(elf,flash,syms,oldelf,backup):
    oi=load_ins(oldelf,0xc0,0x7a00);ni=load_ins(elf,0xc0,syms['__flash_code_end']);addr=lambda n:syms[n]&~1
    rng=random.Random(0x42c21)
    def text(v,p):
        b=bytearray()
        for i in range(80):
            x=v.read(p+i,1)
            if not x:return bytes(b)
            b.append(x)
        raise AssertionError('unterminated text')
    def div64(v):
        n=v.r[0]|v.r[1]<<32;d=v.r[2]|v.r[3]<<32
        n=n if n<1<<63 else n-(1<<64);d=d if d<1<<63 else d-(1<<64)
        q=abs(n)//abs(d)*(-1 if (n<0)!=(d<0) else 1);r=n-q*d
        v.r[:4]=[q&0xffffffff,(q>>32)&0xffffffff,r&0xffffffff,(r>>32)&0xffffffff]
    boundaries=[0,1,-1,18,-18,999,-999,1000,-1000,9999,10000,99999,100000,-100000,10000000,-10000000,0x7fffffff,-0x80000000]
    for k in range(256):
        r=bytearray(rng.getrandbits(8192*8).to_bytes(8192,'little'));p1=rng.randrange(-100000,100001);p2=rng.randrange(-100000,100001)
        d1=boundaries[k%len(boundaries)] if k<128 else rng.randrange(-250000000,250000001)
        d2=boundaries[(k//3)%len(boundaries)] if k<128 else rng.randrange(-250000,250001)
        err=boundaries[(k//5)%len(boundaries)] if k<128 else rng.randrange(-300000,300001)
        for o,x,n in [(0x668,p1,4),(0x664,p2,4),(0x6a0,p1+d1,8),(0x680,p2+d2,8),(0x66c,err,4)]:put(r,o,x,n)
        results=[]
        for legacy,f,ii in [(True,backup,oi),(False,flash,ni)]:
            rows=[]
            def display(v):rows.append(tuple(text(v,p) for p in v.r[:3]))
            callbacks={(0x4884 if legacy else addr('display_three_lines')):('display',display)}
            if legacy:callbacks[0x164]=('div64',div64)
            v=VM(f,ii,r,{},legacy=legacy,callbacks=callbacks);v.execute(0x3220 if legacy else addr('render_telemetry'),limit=200000)
            results.append((rows,v.ram[:0xa54],v.mmio))
        assert results[0]==results[1],('telemetry',k,d1,d2,err,results[0][0],results[1][0])
    print('PASS: 256 relocated telemetry scenarios, displayed strings and workspace compared; original signed 64-bit division modeled.',flush=True)
