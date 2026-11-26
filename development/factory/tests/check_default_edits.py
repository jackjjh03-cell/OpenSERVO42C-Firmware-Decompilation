"""Build an isolated edit and execute settings load to verify edit semantics."""
import pathlib, shutil, tempfile, subprocess, sys
root=pathlib.Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory() as d:
 dest=pathlib.Path(d)/'source';shutil.copytree(root,dest,ignore=shutil.ignore_patterns('__pycache__'))
 p=dest/'include/defaults.h';s=p.read_text().replace('DEFAULT_UART_ADDRESS         0xE0u','DEFAULT_UART_ADDRESS         0xE5u').replace('DEFAULT_MICROSTEPS           16u','DEFAULT_MICROSTEPS           32u');p.write_text(s)
 subprocess.run([sys.executable,str(dest/'build_audit.py'),'--defaults-test'],check=True)
 print('PASS: actual source edit to fallback UART address and microsteps; valid saved settings still take precedence.')
