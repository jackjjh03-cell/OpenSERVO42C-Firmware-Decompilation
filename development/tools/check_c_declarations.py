#!/usr/bin/env python3
"""Detect inconsistent external C function types across translation units.
Uses the same target as the firmware compiler. No binary or hardware needed.
"""
import collections,json,pathlib,subprocess
ROOT=pathlib.Path(__file__).resolve().parents[1]
def check():
    root=ROOT/'factory';seen=collections.defaultdict(lambda:collections.defaultdict(set))
    for path in sorted((root/'src').glob('*.c')):
        ast=json.loads(subprocess.check_output(['clang','--target=arm-none-eabi','-mcpu=cortex-m0plus','-mthumb','-ffreestanding','-fsyntax-only','-I'+str(root/'include'),'-Xclang','-ast-dump=json',str(path)]))
        for node in ast.get('inner',[]):
            if node['kind']=='FunctionDecl' and node.get('storageClass')!='static':
                seen[node['name']][node['type']['qualType']].add(path.name)
    bad={name:types for name,types in seen.items() if len(types)>1}
    if bad:
        for name,types in bad.items():print(name,{t:sorted(p) for t,p in types.items()})
        raise SystemExit('Inconsistent external function declarations')
    print(f'PASS: {len(seen)} external C function names checked across factory translation units; no inconsistent declaration types.')
if __name__=='__main__':check()
