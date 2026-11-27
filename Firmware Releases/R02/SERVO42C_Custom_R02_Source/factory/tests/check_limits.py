"""Negative build tests: the normal toolchain must reject oversized edits.
Works from the standalone source-only project. No hardware access.
"""
import pathlib, shutil, tempfile, subprocess, sys, re
root=pathlib.Path(__file__).resolve().parents[1]
for kind, declaration, statement, diagnostic in [
 ('flash', 'const volatile unsigned char overflow_probe[4096]={1};', 'if(overflow_probe[0]==0)for(;;){}', 'FLASH'),
 ('RAM', 'volatile unsigned char overflow_probe[4096];', 'overflow_probe[0]=1;', 'Globals exceed RAM')]:
 with tempfile.TemporaryDirectory() as td:
  dest=pathlib.Path(td)/'source';shutil.copytree(root,dest,ignore=shutil.ignore_patterns('__pycache__'))
  p=dest/'src/reset.c';s,n=re.subn(r'void reset_handler\(void\)\s*\{',lambda _: declaration+'\nvoid reset_handler(void) {'+statement,p.read_text(),count=1);assert n==1;p.write_text(s)
  r=subprocess.run([sys.executable,str(dest/'build_audit.py')],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
  assert r.returncode!=0 and diagnostic in r.stdout and ('overflow' in r.stdout or 'will not fit' in r.stdout or 'Globals exceed RAM' in r.stdout),r.stdout
  print(f'PASS: deliberate {kind} overflow rejected by linker.')
