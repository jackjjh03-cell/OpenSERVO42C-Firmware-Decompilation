#!/usr/bin/env python3
"""SERVO42C Custom R02 UART settings viewer and explicit tuning editor.
Requires Python 3.8+, pyserial, and Tkinter for the GUI. No motion commands.
"""
import argparse

FIELDS = [
    'Control mode','UART baud','UART address','Microsteps','Current raw','Direction',
    'Enable mode','Protection','Interpolation','Auto display sleep','Alternate microsteps',
    'Motor geometry raw','Acceleration raw','Torque raw','Error limit raw','Origin mode',
    'Origin speed index','Origin direction','Origin position raw','Saved motion raw','Acceleration index',
    'Kp raw','Ki raw','Kd raw','Drive enabled','UART motion state','Stall flag',
    'Position error (raw 32-bit)','Pulse position low word','Target low word','UART speed raw',
    'UART enable request','Control output (raw 32-bit)','Encoder angle raw','Encoder status',
    'Encoder LUT ready','Knot valid','LUT valid','Feedback position low word','Startup state',
    'Stall flag (faults)','Protection (faults)','ADC feedback 1 raw','ADC feedback 2 raw','Phase index',
]
# opcode, minimum, maximum, factory fallback (not a recommended motor-specific tuning)
TUNING={21:(0xA1,0,65535,1638),22:(0xA2,0,65535,1),23:(0xA3,0,65535,1638),
        12:(0xA4,0,65535,286),13:(0xA5,0,1200,1080)}

def packet(address,opcode,*args):
    data=bytes((address,opcode,*args));return data+bytes((sum(data)&255,))

def parse_reply(data,address,n):
    if len(data)!=n+2 or data[0]!=address or (sum(data[:-1])&255)!=data[-1]:
        raise IOError('Invalid or incomplete response; check mode, baud, address and wiring. No automatic retry.')
    return int.from_bytes(data[1:-1],'big')

class Controller:
    def __init__(self,port,address,echo=False):self.port=port;self.address=address;self.echo=echo
    def exchange(self,opcode,args,n):
        frame=packet(self.address,opcode,*args)
        self.port.reset_input_buffer();self.port.write(frame);self.port.flush()
        if self.echo and self.port.read(len(frame))!=frame:raise IOError('Serial echo did not match the request.')
        return parse_reply(self.port.read(n+2),self.address,n)
    def query(self,index):
        if not 0<=index<len(FIELDS):raise ValueError('Unknown field')
        return self.exchange(0x8D,[index],4)
    def set_tuning(self,index,value):
        if index not in TUNING:raise ValueError('Not an editable tuning field')
        op,lo,hi,_=TUNING[index]
        if not lo<=value<=hi:raise ValueError('Value outside raw protocol range')
        # Detect active or commanded motion. This is an advisory software gate,
        # not a replacement for independently isolating the drive and motor.
        if self.query(0)!=2 or any(self.query(i)!=0 for i in (24,25,31)) or self.exchange(0x3A,[],1)!=2:
            raise RuntimeError('Write refused: CR_UART, disabled drive and idle motion are required.')
        current=self.query(index)
        if current==value:return current
        status=self.exchange(op,[(value>>8)&255,value&255],1)
        if status!=1:raise IOError('Controller did not acknowledge the settings command.')
        # Acknowledgment and readback are not proof that flash survived power loss.
        readback=self.query(index)
        if readback!=value:raise IOError('Readback differs. Do not retry automatically; inspect the controller.')
        return readback

def display_value(index,v):
    if index==0:return {0:'CR_OPEN',1:'CR_vFOC',2:'CR_UART'}.get(v,str(v))
    if index==2:return '0x%02X'%v
    if index in (27,28,29,32,38,42,43):return '0x%08X'%v
    return str(v)

def gui(controller):
    import tkinter as tk
    from tkinter import ttk,messagebox
    root=tk.Tk();root.title('SERVO42C — Configuration');root.geometry('690x600')
    outer=ttk.Frame(root,padding=10);outer.pack(fill='both',expand=True)
    ttk.Label(outer,text='SysInfo / live configuration',font=('TkDefaultFont',12,'bold')).pack(anchor='w')
    ttk.Label(outer,text='Read-only until an explicit, confirmed tuning write. Drive must be disabled.',wraplength=650).pack(anchor='w',pady=(4,8))
    frame=ttk.Frame(outer);frame.pack(fill='both',expand=True)
    view=ttk.Treeview(frame,columns=('field','value'),show='headings',height=14)
    view.heading('field',text='Field');view.heading('value',text='Live value');view.column('field',width=360);view.column('value',width=180)
    scroll=ttk.Scrollbar(frame,orient='vertical',command=view.yview);view.configure(yscrollcommand=scroll.set);view.pack(side='left',fill='both',expand=True);scroll.pack(side='right',fill='y')
    for i,name in enumerate(FIELDS):view.insert('','end',iid=str(i),values=(name,'—'))
    status=tk.StringVar(value='Not read yet');ttk.Label(outer,textvariable=status,wraplength=650).pack(anchor='w',pady=6)
    def refresh():
        try:
            for i in range(len(FIELDS)):
                v=controller.query(i);view.item(str(i),values=(FIELDS[i],display_value(i,v)))
            status.set('Read complete. Values sampled sequentially, not an atomic snapshot.')
        except Exception as ex:status.set(str(ex));messagebox.showerror('Read failed',str(ex))
    ttk.Button(outer,text='Refresh',command=refresh).pack(anchor='w')
    edit=ttk.LabelFrame(outer,text='Tuning (raw values)',padding=8);edit.pack(fill='x',pady=(10,0))
    options=list(TUNING);choice=tk.StringVar(value=FIELDS[options[0]]);value=tk.StringVar()
    ttk.Combobox(edit,textvariable=choice,values=[FIELDS[i] for i in options],state='readonly',width=26).grid(row=0,column=0,padx=4,pady=4)
    ttk.Entry(edit,textvariable=value,width=12).grid(row=0,column=1,padx=4,pady=4)
    def index():return FIELDS.index(choice.get())
    def default():value.set(str(TUNING[index()][3]))
    def apply():
        try:
            i=index();v=int(value.get(),0);_,lo,hi,_=TUNING[i]
            if not lo<=v<=hi:raise ValueError('Raw range: %d–%d'%(lo,hi))
            if not messagebox.askyesno('Confirm persistent write',
                'Write %s = %d to flash?\n\nConfirm the drive is disabled, motor power/motion is safely isolated, and no other controller is issuing commands. Raw ranges do not imply safe tuning. One write will be attempted.'%(FIELDS[i],v)):return
            got=controller.set_tuning(i,v);view.item(str(i),values=(FIELDS[i],str(got)));status.set('Readback matched. Power-cycle persistence not yet verified.')
        except Exception as ex:status.set(str(ex));messagebox.showerror('Write failed',str(ex))
    ttk.Button(edit,text='Factory value',command=default).grid(row=0,column=2,padx=4)
    ttk.Button(edit,text='Write',command=apply).grid(row=0,column=3,padx=4)
    ttk.Label(edit,text='Raw values are not safe tuning limits. Factory values are reference defaults, not universal motor settings.',wraplength=610).grid(row=1,column=0,columnspan=4,sticky='w')
    root.mainloop()

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('port');p.add_argument('--baud',type=int,default=115200);p.add_argument('--address',type=lambda s:int(s,0),default=0xE0);p.add_argument('--echo',action='store_true');p.add_argument('--dump',action='store_true');a=p.parse_args()
    if not 0xe0<=a.address<=0xe9:p.error('Address must be 0xE0..0xE9')
    import serial
    with serial.Serial(a.port,a.baud,timeout=1.0,write_timeout=1.0) as port:
        c=Controller(port,a.address,a.echo)
        if a.dump:
            for i,name in enumerate(FIELDS):print('%02d %-30s %s'%(i,name,display_value(i,c.query(i))))
        else:gui(c)
if __name__=='__main__':main()
