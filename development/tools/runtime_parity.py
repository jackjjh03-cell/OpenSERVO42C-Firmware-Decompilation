#!/usr/bin/env python3
"""Compare the actual factory and rebuilt Thumb arithmetic; no hardware access."""
import argparse, hashlib, pathlib, random, tempfile
from audit_factory import nm, wrap_original
from inspect_elf import read_application
from check_backup import read_backup
from integration import VM, put
from thumb_model import load_ins
MASK=0xffffffff
class ExactArithmeticVM(VM):
    def step(self,p):
        if self.legacy_addresses and (0x110<=p<0x22a or 0x292<=p<0x312 or 0x338<=p<0x35a):
            self.legacy_addresses=False
            try:return super().step(p)
            finally:self.legacy_addresses=True
        return super().step(p)
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('elf',type=pathlib.Path);p.add_argument('backup',type=pathlib.Path);p.add_argument('--random',type=int,default=128);a=p.parse_args()
    old=read_backup(a.backup);app,_=read_application(a.elf);s=nm(a.elf);addr=lambda n:s[n]&~1
    assert hashlib.sha256(old[:0x7a00]).hexdigest()=='1a64209c9cb46679f78628b8eb82cddcb5ef78972b9f6699a9b41aeb12610595'
    new=app+old[0x7a00:];print('Application SHA256:',hashlib.sha256(app).hexdigest(),flush=True)
    rng=random.Random(421103)
    with tempfile.TemporaryDirectory() as t:
        oe=wrap_original(old,pathlib.Path(t));oi=load_ins(oe,0xc0,0x7a00);ni=load_ins(a.elf,0xc0,s['__flash_code_end'])
        for name,entry,width in [('unsigned_divmod_pair',0x110,32),('signed_divmod_pair',0x13c,32),('factory_multiply64',0x1b0,64)]:
            edges=[0,1,2,3,(1<<(width-1))-1,1<<(width-1),(1<<width)-2,(1<<width)-1]
            cases=[(x,y) for x in edges for y in edges]+[(rng.getrandbits(width),rng.getrandbits(width)) for _ in range(a.random)]
            for x,y in cases:
                args=[x,y,0,0] if width==32 else [x&MASK,x>>32,y&MASK,y>>32]
                result=[]
                for legacy,f,ii,ep in [(True,old,oi,entry),(False,new,ni,addr(name))]:
                    v=ExactArithmeticVM(f,ii,bytes(8192),{},legacy=legacy);v.r[:4]=args;v.execute(ep);result.append(tuple(v.r[:2]));assert v.r[13]==0x200015f8
                assert result[0]==result[1],(name,x,y,result)
            print(f'PASS: {len(cases)} {name} comparisons; Thumb executed on both sides, including zero divisors where applicable.',flush=True)
        edges=[0,1,2,3,0x7fffffffffffffff,0x8000000000000000,0xfffffffffffffffe,0xffffffffffffffff]
        cases=[(x,y) for x in edges for y in edges]+[(rng.getrandbits(64),rng.getrandbits(64)) for _ in range(a.random)]
        for x,y in cases:
            for signed,ep in [(False,0x292),(True,0x164)]:
                v=ExactArithmeticVM(old,oi,bytes(8192),{},legacy=True);v.r[:4]=[x&MASK,x>>32,y&MASK,y>>32];v.execute(ep)
                n=x-(1<<64) if signed and x>>63 else x;d=y-(1<<64) if signed and y>>63 else y
                q=abs(n)//abs(d) if d else (1<<64)-1
                if (n<0)!=(d<0):q=-q
                r=n-q*d if d else n
                expected=(q&((1<<64)-1),r&((1<<64)-1));got=(v.r[0]|v.r[1]<<32,v.r[2]|v.r[3]<<32)
                assert expected==got,(signed,hex(x),hex(y),expected,got)
        print(f'PASS: {2*len(cases)} original 64-bit quotient/remainder executions against mathematical oracle.',flush=True)
        def text(v,p):
            b=bytearray()
            for i in range(80):
                x=v.read(p+i,1)
                if not x:return bytes(b)
                b.append(x)
            raise AssertionError('unterminated text')
        bounds=[0,1,-1,18,-18,999,-999,1000,-1000,9999,10000,99999,100000,-100000,10000000,-10000000,0x7fffffff,-0x80000000]
        for k in range(256):
            r=bytearray(rng.getrandbits(8192*8).to_bytes(8192,'little'));p1=rng.randrange(-100000,100001);p2=rng.randrange(-100000,100001)
            d1=bounds[k%len(bounds)] if k<128 else rng.randrange(-250000000,250000001)
            d2=bounds[(k//3)%len(bounds)] if k<128 else rng.randrange(-250000,250001)
            err=bounds[(k//5)%len(bounds)] if k<128 else rng.randrange(-300000,300001)
            for o,x,n in [(0x668,p1,4),(0x664,p2,4),(0x6a0,p1+d1,8),(0x680,p2+d2,8),(0x66c,err,4)]:put(r,o,x,n)
            results=[]
            for legacy,f,ii in [(True,old,oi),(False,new,ni)]:
                rows=[]
                def display(v):rows.append(tuple(text(v,p) for p in v.r[:3]))
                callbacks={(0x4884 if legacy else addr('display_three_lines')):('display',display)}
                v=ExactArithmeticVM(f,ii,r,{},legacy=legacy,callbacks=callbacks);v.execute(0x3220 if legacy else addr('render_telemetry'),limit=200000)
                results.append((rows,v.ram[:0xa54],v.mmio))
            assert results[0]==results[1],('telemetry',k,d1,d2,err,results[0][0],results[1][0])
        print('PASS: 256 telemetry comparisons; factory arithmetic instructions executed, OLED output mocked.',flush=True)
if __name__=='__main__':main()
