#!/usr/bin/env python3
"""Decode all eight V1.1.3 configuration slots in a complete HEX/BIN backup.
Read-only. Does not change settings, firmware or the attached board.
"""
import argparse, hashlib, json, pathlib, struct, sys
ROOT=pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'factory'/'tests'))
from check_backup import read_backup
MODES={0:'CR_OPEN',1:'CR_vFOC',2:'CR_UART'}
FIELDS=[('microsteps',0,2),('alternate_microsteps',2,2),('direction_raw',4,1),('error_limit_raw',6,2),('enable_mode_raw',8,1),('mode_raw',9,1),('autosdd_raw',10,1),('protection_raw',11,1),('interpolation_raw',12,1),('baud',16,4),('uart_address',20,2),('motion_saved_raw',22,2),('acceleration_index',24,2),('torque_raw',26,2),('current_raw',28,2),('half_current_raw',30,2),('motor_geometry_raw',32,2),('origin_mode',34,1),('origin_position',36,2),('origin_speed',38,2),('origin_direction',40,1),('kp_raw',42,2),('ki_raw',44,2),('kd_raw',46,2),('acceleration_raw',48,2),('unknown32',50,2),('unknown34',52,2),('valid_marker',54,2)]
def decode(backup):
    if len(backup)!=65536:raise ValueError('Expected 64 KiB')
    records=[]
    selected=0
    for i in range(8):
        a=0x7c00+i*64; data=backup[a:a+64]; row={'slot':i,'address':f'0x{a:04X}','empty':data==b'\xff'*64}
        if int.from_bytes(data[:2],'little')!=65535:selected=i
        if not row['empty']:
            row.update({name:int.from_bytes(data[off:off+n],'little') for name,off,n in FIELDS})
            row['mode']=MODES.get(row['mode_raw'],'unknown');row['record_hex']=data.hex();row['marked_valid']=row['valid_marker']==1
        records.append(row)
    # Factory 0x4660 selects the last slot with a non-erased FIRST halfword;
    # 0x5368 checks its marker afterwards. Do not implement enhanced fallback.
    active=records[selected]
    return {'sha256':hashlib.sha256(backup).hexdigest(),'factory_selected_slot':selected,'factory_selected_address':active['address'],'uses_saved_record':active.get('marked_valid',False),'records':records}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('backup');p.add_argument('--json',type=pathlib.Path);a=p.parse_args()
    result=decode(read_backup(a.backup));print('Selected:',result['factory_selected_address'],'valid:',result['uses_saved_record'])
    print('Address   Mode       Baud     UART   Kp(raw)  Valid')
    for r in result['records']:
        if r['empty']: print(f"{r['address']}    erased");continue
        print(f"{r['address']}    {r['mode']:<10} {r['baud']:<8} 0x{r['uart_address']:02X}   {r['kp_raw']:<8} {r['marked_valid']}"+('  ACTIVE' if r['slot']==result['factory_selected_slot'] else ''))
    if a.json:a.json.write_text(json.dumps(result,indent=2)+'\n')
if __name__=='__main__':main()
