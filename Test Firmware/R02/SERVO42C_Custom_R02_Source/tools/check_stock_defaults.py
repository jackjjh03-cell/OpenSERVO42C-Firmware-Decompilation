#!/usr/bin/env python3
"""Check the unedited factory baseline. Not a restriction on custom builds."""
import pathlib,re
p=pathlib.Path(__file__).resolve().parents[1]/'factory/include/defaults.h'
s=p.read_text()
for name,value in {'DEFAULT_CONTROL_MODE':1,'DEFAULT_UART_BAUD_RAW':0,'DEFAULT_UART_ADDRESS':0xe0,'DEFAULT_KP':1638}.items():
    m=re.search(r'^#define\s+'+name+r'\s+(0x[0-9a-fA-F]+|[0-9]+)u?\s*$',s,re.M)
    assert m and int(m[1],0)==value,name
print('PASS: stock CR_vFOC, UART disabled, address E0, Kp 1638. User settings not compiled into defaults.')
