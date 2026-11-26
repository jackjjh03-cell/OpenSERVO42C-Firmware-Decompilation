#!/usr/bin/env python3
"""Check baseline source locations and linked addresses (not semantic proof)."""
import csv,pathlib,re,sys
from audit_factory import nm
root=pathlib.Path(__file__).resolve().parents[1]
rows=list(csv.DictReader((root/'docs/function_traceability.csv').open()))
syms=nm(sys.argv[1]);assert len(rows)==len({r['original_address'] for r in rows})==270
for r in rows:
    file,line=r['source_location'].rsplit(':',1);name=r['reconstructed_symbol'];lines=(root/file).read_text().splitlines()
    assert re.search(r'\b'+re.escape(name)+r'\s*\(',lines[int(line)-1]),r
    if r['linked_address']:assert syms[name]&~1==int(r['linked_address'],16),r
print('PASS: all 270 baseline function mappings, source lines and recorded linked addresses checked.')
