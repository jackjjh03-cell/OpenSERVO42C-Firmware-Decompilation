"""OpenSERVO42C Studio development interface (Python 3.8 / Tk 8.6)."""
import csv
import time
import tkinter as tk
from collections import deque
from concurrent.futures import ThreadPoolExecutor
from tkinter import ttk, messagebox, filedialog
from . import protocol as p
from .client import ControllerClient, MODE_NAMES
from .tuning import analyze

GROUPS = (
    ("Protection", p.GET_CONFIG, p.SET_CONFIG, ("Following error (counts)", "Stall error (counts)", "Stall speed (counts/s)", "Stall load (permille)", "Fault dwell (ms)", "Clear dwell (ms)", "Policy (0 monitor, 1 warn, 2 protect)")),
    ("PID", p.GET_PID, p.SET_PID, ("Kp (Q12)", "Ki (Q12)", "Kd (Q12)", "Deadband (counts)", "Output limit")),
    ("Motion", p.GET_MOTION, p.SET_MOTION, ("Speed (counts/s)", "Acceleration (counts/s²)", "Deceleration (counts/s²)", "Jerk (counts/s³)", "Curve (0 trapezoid, 1 S)")),
    ("Drive", p.GET_DRIVE, p.SET_DRIVE, ("Microsteps", "Max encoder delta", "Phase current (raw)", "Current gain (Q11)", "PWM compare limit", "Geometry (25 or 50)", "Reverse (0 or 1)", "Trip current (raw)", "Enable active high (0 or 1)")),
    ("Travel", p.GET_LIMITS, p.SET_LIMITS, ("Minimum (counts)", "Maximum (counts)", "Enabled (0 or 1)")),
)
FAULTS = ("Stall", "Following error", "Travel", "Encoder", "Electrical", "Timing", "Reference")


def fault_names(mask):
    names = [name for i, name in enumerate(FAULTS) if mask & (1 << i)]
    if mask & ~127:
        names.append("Unknown 0x{:X}".format(mask & ~127))
    return ", ".join(names) or "None"


class Studio:
    def __init__(self, root, opener=ControllerClient.open):
        self.root, self.opener = root, opener
        self.worker = ThreadPoolExecutor(max_workers=1)
        self.client = None  # accessed by serial worker only
        self.connected = False
        self.future = None
        self.pending = None
        self.closing = False
        self.snapshot = None
        self.history = deque(maxlen=1200)
        self.config_vars = []
        self.start = time.monotonic()
        root.title("OpenSERVO42C Studio — development")
        root.geometry("980x700")
        style = ttk.Style(root)
        style.theme_use("clam")
        style.configure("TFrame", background="#20252d")
        style.configure("TLabel", background="#20252d", foreground="#e2e8ef")
        style.configure("TLabelframe", background="#20252d", foreground="#e2e8ef")
        style.configure("TLabelframe.Label", background="#20252d", foreground="#e2e8ef")
        style.configure("TNotebook", background="#20252d")
        style.configure("TNotebook.Tab", background="#303844", foreground="#e2e8ef")
        style.map("TNotebook.Tab", background=[("selected", "#47576b")])
        root.configure(background="#20252d")
        bar = ttk.Frame(root, padding=10); bar.pack(fill="x")
        self.port = tk.StringVar(value="COM3")
        ttk.Label(bar, text="Serial port").pack(side="left")
        ttk.Entry(bar, textvariable=self.port, width=15).pack(side="left", padx=5)
        ttk.Button(bar, text="Connect", command=self.connect).pack(side="left")
        ttk.Button(bar, text="Disconnect", command=self.disconnect).pack(side="left", padx=4)
        ttk.Button(bar, text="Disable output", command=self.disable).pack(side="right")
        self.message = tk.StringVar(value="Disconnected — use an independent hardware stop.")
        ttk.Label(root, textvariable=self.message, padding=10, wraplength=940).pack(fill="x")
        tabs = ttk.Notebook(root); tabs.pack(fill="both", expand=True, padx=10, pady=5)
        monitor = ttk.Frame(tabs, padding=12); tabs.add(monitor, text="Monitor")
        self.status_text = tk.StringVar(value="No controller data")
        ttk.Label(monitor, textvariable=self.status_text, justify="left").pack(anchor="w")
        self.graph = tk.Canvas(monitor, background="#151a21", highlightthickness=0, height=280)
        self.graph.pack(fill="both", expand=True, pady=10)
        self.graph.bind("<Configure>", lambda _: self.draw_graph())
        ttk.Label(monitor, text="Position: actual (blue), target (orange) · encoder counts").pack(anchor="w")
        ttk.Button(monitor, text="Export recent samples to CSV", command=self.export_csv).pack(anchor="w", pady=5)
        motion = ttk.Frame(tabs, padding=12); tabs.add(motion, text="Motion")
        self.mode = tk.StringVar(value=MODE_NAMES[0])
        ttk.Combobox(motion, textvariable=self.mode, values=MODE_NAMES, state="readonly", width=18).grid(row=0, column=0, sticky="w")
        ttk.Button(motion, text="Select mode", command=lambda: self.command(p.SELECT_MODE, MODE_NAMES.index(self.mode.get()))).grid(row=0, column=1)
        ttk.Button(motion, text="Arm", command=self.arm).grid(row=1, column=0, sticky="w", pady=8)
        ttk.Button(motion, text="Acknowledge fault", command=lambda: self.command(p.ACK)).grid(row=1, column=1)
        self.position = self.field(motion, 2, "Absolute target (counts)", "0")
        ttk.Button(motion, text="Move", command=lambda: self.numeric_command(p.MOVE_ABS, self.position)).grid(row=2, column=2)
        self.rate = self.field(motion, 3, "Signed rate (counts/s)", "0")
        self.lease = self.field(motion, 4, "Jog/torque lease (ms)", "250")
        ttk.Button(motion, text="Set velocity", command=lambda: self.numeric_command(p.SET_RATE, self.rate, 0)).grid(row=3, column=2)
        ttk.Button(motion, text="Jog for lease", command=lambda: self.numeric_command(p.SET_RATE, self.rate, self.lease)).grid(row=4, column=2)
        self.torque = self.field(motion, 5, "Signed torque (raw current)", "0")
        ttk.Button(motion, text="Apply torque for lease", command=lambda: self.numeric_command(p.SET_TORQUE, self.torque, self.lease)).grid(row=5, column=2)
        ttk.Button(motion, text="Zero velocity/jog demand", command=lambda: self.command(p.SET_RATE, 0, 0)).grid(row=6, column=0, pady=8)
        ttk.Button(motion, text="Zero torque demand", command=lambda: self.command(p.SET_TORQUE, 0, 0)).grid(row=6, column=1)
        ttk.Label(motion, text="Development firmware: do not use on a loaded machine.\nSerial disable is not a hardware emergency stop.", wraplength=800).grid(row=7, column=0, columnspan=3, sticky="w", pady=10)
        settings = ttk.Frame(tabs, padding=12); tabs.add(settings, text="Settings")
        inner = ttk.Notebook(settings); inner.pack(fill="both", expand=True)
        for name, get, put, labels in GROUPS:
            panel = ttk.Frame(inner, padding=10); inner.add(panel, text=name)
            variables = [self.field(panel, i, label, "") for i, label in enumerate(labels)]
            self.config_vars.append(variables)
            ttk.Button(panel, text="Read controller", command=lambda g=get,v=variables: self.submit(lambda c: c.read(g), lambda data: self.fill(v,data))).grid(row=len(labels), column=0, sticky="w", pady=10)
            ttk.Button(panel, text="Apply to RAM", command=lambda o=put,v=variables: self.numeric_command(o,*v)).grid(row=len(labels), column=1, sticky="w")
        ttk.Button(settings, text="Save current configuration to controller", command=self.save).pack(anchor="w", pady=8)
        teach = ttk.Frame(tabs, padding=12); tabs.add(teach, text="Teach travel")
        ttk.Label(teach, text="Disable motor output and external EN. Establish a known reference, then move the mechanism by hand to each endpoint. Apply limits before saving. Saving invalidates the position reference.", wraplength=850).grid(row=0, column=0, columnspan=3, sticky="w", pady=8)
        self.reference = self.field(teach, 1, "Known reference (counts)", "0")
        ttk.Button(teach, text="Confirm reference", command=lambda: self.numeric_command(p.CONFIRM_REFERENCE, self.reference)).grid(row=1, column=2)
        ttk.Button(teach, text="Begin teaching", command=self.begin_teach).grid(row=2,column=0,sticky="w",pady=8)
        self.captured = [tk.StringVar(value="Not captured"), tk.StringVar(value="Not captured")]
        for i, name in enumerate(("Minimum", "Maximum")):
            ttk.Button(teach, text="Capture " + name, command=lambda i=i: self.submit(lambda c: c.capture_limit(i==1), lambda data: self.captured[i].set(str(data)))).grid(row=3+i,column=0,sticky="w",pady=4)
            ttk.Label(teach, textvariable=self.captured[i]).grid(row=3+i,column=1,sticky="w")
        self.lower = self.field(teach, 5, "Lower inward margin (counts)", "0")
        self.upper = self.field(teach, 6, "Upper inward margin (counts)", "0")
        ttk.Button(teach, text="Apply taught limits", command=lambda: self.numeric_command(p.TEACH_APPLY,self.lower,self.upper)).grid(row=7,column=0,sticky="w",pady=8)
        ttk.Button(teach, text="Cancel teaching", command=self.cancel_teach).grid(row=7,column=1)
        ttk.Label(teach, textvariable=self.status_text, justify="left", wraplength=850).grid(row=8,column=0,columnspan=3,sticky="w",pady=12)
        diagnostics = ttk.Frame(tabs, padding=12); tabs.add(diagnostics, text="Diagnostics")
        self.calibration_text = tk.StringVar(value="Calibration not inspected")
        ttk.Button(diagnostics, text="Inspect calibration", command=lambda: self.submit(lambda c: c.read(p.GET_CALIBRATION), self.show_calibration)).pack(anchor="w")
        ttk.Label(diagnostics, textvariable=self.calibration_text, justify="left", wraplength=850).pack(anchor="w", pady=10)
        tuning = ttk.Frame(tabs, padding=12); tabs.add(tuning, text="Tuning trace")
        self.trace_count = self.field(tuning, 0, "Samples (1–160)", "160")
        self.trace_interval = self.field(tuning, 1, "Sample interval (ms)", "5")
        self.trace_tolerance = self.field(tuning, 2, "Settling tolerance (counts)", "16")
        self.trace_text = tk.StringVar(value="Start recording, command one position move, then disable output before retrieving the trace.")
        self.trace_rows = []
        ttk.Button(tuning,text="Start trace",command=lambda: self.numeric_command(p.TRACE_START,self.trace_count,self.trace_interval)).grid(row=3,column=0,sticky="w")
        ttk.Button(tuning,text="Stop trace",command=lambda: self.command(p.TRACE_STOP)).grid(row=3,column=1,sticky="w")
        ttk.Button(tuning,text="Trace status",command=lambda: self.submit(lambda c:c.read(p.TRACE_STATUS),self.show_trace_status)).grid(row=4,column=0,sticky="w")
        ttk.Button(tuning,text="Retrieve and analyze",command=self.get_trace).grid(row=4,column=1,sticky="w")
        ttk.Button(tuning,text="Export trace CSV",command=self.export_trace).grid(row=5,column=0,sticky="w")
        ttk.Label(tuning,textvariable=self.trace_text,justify="left",wraplength=850).grid(row=6,column=0,columnspan=3,sticky="w",pady=12)
        root.protocol("WM_DELETE_WINDOW", self.close)
        root.after(50, self.pump)
        self.next_poll = 0

    @staticmethod
    def field(parent, row, label, initial):
        ttk.Label(parent, text=label).grid(row=row,column=0,sticky="w",padx=4,pady=3)
        var = tk.StringVar(value=initial)
        ttk.Entry(parent, textvariable=var, width=18).grid(row=row,column=1,sticky="w",padx=5)
        return var

    @staticmethod
    def fill(variables, values):
        for var, val in zip(variables, values):
            var.set(str(val))

    def submit(self, action, done=None, quiet=False, urgent=False):
        if not self.connected:
            if not quiet: self.message.set("Connect first.")
            return
        if self.future is not None:
            if not quiet and (urgent or (self.quiet and self.pending is None)):
                self.pending = (action, done, urgent)
                self.message.set("Command queued after current transaction.")
            elif not quiet:
                self.message.set("Serial transaction in progress. Hardware stop remains independent.")
            return
        self.future = self.worker.submit(lambda: action(self.client))
        self.done, self.quiet = done, quiet

    def disable(self):
        self.submit(lambda c: c.write(p.DISABLE), urgent=True)

    def reset_captures(self, _):
        for var in self.captured: var.set("Not captured")

    def begin_teach(self):
        self.submit(lambda c: c.write(p.TEACH_BEGIN), self.reset_captures)

    def cancel_teach(self):
        self.submit(lambda c: c.write(p.TEACH_CANCEL), self.reset_captures)

    def connect(self):
        if self.connected or self.future is not None:
            return
        device = self.port.get().strip()
        def opening():
            client = self.opener(device)
            try:
                client.read(p.HELLO)
            except Exception:
                client.close(); raise
            self.client = client
        self.future = self.worker.submit(opening)
        self.done, self.quiet = self.on_connected, False

    def on_connected(self, _):
        self.connected = True; self.snapshot = None; self.history.clear()
        self.message.set("Connected. No motor commands sent.")

    def disconnect(self):
        if not self.connected or self.future is not None:
            return
        if (not self.snapshot or self.snapshot[0]["state"] == 2) and not messagebox.askyesno("Disconnect", "Disconnecting does not stop the motor. Continue?"):
            return
        self.submit(lambda c: c.close(), self.on_disconnected)

    def on_disconnected(self, _):
        self.connected = False; self.snapshot = None
        self.status_text.set("Disconnected — motor state unknown")

    def command(self, opcode, *values):
        self.submit(lambda c: c.write(opcode,*values))

    def numeric_command(self, opcode, *values):
        try:
            parsed = tuple(int(v.get()) if isinstance(v, tk.StringVar) else int(v) for v in values)
        except ValueError:
            self.message.set("Enter valid integer values first."); return
        self.command(opcode,*parsed)

    def arm(self):
        if messagebox.askyesno("Arm motor", "Confirm safe clearance, current settings and independent stop. Enable motor output?"):
            self.command(p.ARM)

    def save(self):
        if messagebox.askyesno("Save settings", "Motor must be disabled. Saving pauses feedback and invalidates the position reference. Continue?"):
            self.command(p.SAVE_SETTINGS)

    def pump(self):
        if self.future is not None and self.future.done():
            failed = False
            future, done, quiet = self.future, self.done, self.quiet
            self.future = None
            try:
                value = future.result()
                if not quiet: self.message.set("Command completed.")
                if done: done(value)
            except Exception as exc:
                failed = True
                self.message.set(str(exc)); self.snapshot = None
                self.status_text.set("Controller state unknown — verify output independently.")
            if self.pending is not None:
                action, callback, urgent = self.pending; self.pending = None
                if urgent or (not failed and not self.closing):
                    self.submit(action, callback, urgent=urgent)
        if self.closing:
            if self.future is None:
                if self.connected:
                    self.submit(lambda c: c.close(), self.on_disconnected)
                    self.connected = False
                else:
                    self.worker.shutdown(wait=False); self.root.destroy(); return
        elif self.connected and self.future is None and time.monotonic() >= self.next_poll:
            self.next_poll = time.monotonic() + 0.25
            self.submit(lambda c: (c.status(),c.run_status()), self.update_monitor, True)
        self.root.after(50, self.pump)

    def update_monitor(self, data):
        self.snapshot = data
        s, r = data
        states = ("Off", "Ready", "Active", "Fault")
        state = states[s["state"]] if s["state"] < len(states) else "Unknown"
        mode = MODE_NAMES[s["mode"]] if s["mode"] < len(MODE_NAMES) else "Unknown"
        self.status_text.set("{} · {} · PWM {} · EN {} · Reference {}\nActual {:,}   Target {:,}   Speed {:,} counts/s   Load {:.1f}%\nFaults: {}   Pending: {}   Missed ticks: {}".format(state,mode,"on" if r["flags"]&1 else "off","on" if r["flags"]&2 else "off","valid" if r["flags"]&32 else "unconfirmed",s["actual"],s["target"],s["speed_counts_s"],s["load_permille"]/10,fault_names(s["faults"]),fault_names(r["pending_fault"]),r["missed_ticks"]))
        self.history.append((time.monotonic()-self.start,s["actual"],s["target"],s["speed_counts_s"],s["load_permille"]))
        self.draw_graph()

    def show_calibration(self, data):
        winding, jump, forward, reverse, flat, flags = data
        self.calibration_text.set("Structural check: {}\nWinding: {} counts\nLargest adjacent change: {} counts\nForward: {}  Reverse: {}  Flat: {}\nMarkers: {}  Half-turn ambiguity: {}\nRead-only check; does not establish physical calibration accuracy.".format("passed" if flags == 3 else "failed",winding,jump,forward,reverse,flat,"valid" if flags&1 else "invalid","yes" if flags&4 else "no"))

    def show_trace_status(self,data):
        count,requested,interval,flags=data
        self.trace_text.set("{} / {} samples at {} ms · {}{}".format(count,requested,interval,"recording" if flags&1 else "stopped"," · faulted" if flags&2 else ""))

    def get_trace(self):
        try: tolerance=int(self.trace_tolerance.get())
        except ValueError:
            self.message.set("Enter a valid settling tolerance."); return
        if tolerance<1:
            self.message.set("Tolerance must be positive."); return
        def retrieve(c):
            if c.run_status()["flags"]&1:
                raise ValueError("Disable motor output before downloading a trace")
            return c.read_trace()
        def display(data):
            rows,interval,faulted=data; self.trace_rows=rows
            try:
                result=analyze(rows,interval,faulted,tolerance)
                self.trace_text.set("\n".join("{}: {}".format(k.replace("_"," "),round(v,2) if isinstance(v,float) else v if v is not None else "not settled") for k,v in result.items()))
            except ValueError as exc: self.trace_text.set(str(exc)+". Raw trace can still be exported.")
        self.submit(retrieve,display)

    def export_trace(self):
        if not self.trace_rows:
            self.message.set("Retrieve a trace first."); return
        path=filedialog.asksaveasfilename(defaultextension=".csv",filetypes=[("CSV","*.csv")])
        if path:
            try:
                with open(path,"w",newline="",encoding="utf-8") as f:
                    writer=csv.writer(f);writer.writerow(("tick_ms","actual","target","demand","load_permille"));writer.writerows(self.trace_rows)
            except OSError as exc: self.message.set(str(exc))

    def draw_graph(self):
        canvas = self.graph; canvas.delete("all")
        rows = list(self.history)[-240:]
        if len(rows)<2: return
        w,h = max(canvas.winfo_width()-12,1), max(canvas.winfo_height()-24,1)
        lo = min(min(r[1],r[2]) for r in rows); hi = max(max(r[1],r[2]) for r in rows)
        if hi==lo: hi+=1
        for col,color in ((1,"#66b8f5"),(2,"#eea34a")):
            points = [coord for i,r in enumerate(rows) for coord in (6+w*i/(len(rows)-1), 12+h*(hi-r[col])/(hi-lo))]
            canvas.create_line(*points,fill=color,width=2)
        canvas.create_text(8,8,text=str(hi),fill="#b4c0ce",anchor="nw")
        canvas.create_text(8,h+12,text=str(lo),fill="#b4c0ce",anchor="sw")

    def export_csv(self):
        path = filedialog.asksaveasfilename(defaultextension=".csv",filetypes=[("CSV","*.csv")])
        if path:
            try:
                with open(path,"w",newline="",encoding="utf-8") as f:
                    writer=csv.writer(f);writer.writerow(("elapsed_s","actual","target","speed_counts_s","load_permille"));writer.writerows(self.history)
            except OSError as exc: self.message.set(str(exc))

    def close(self):
        if self.connected and not messagebox.askyesno("Exit", "Exiting does not stop the motor. Continue?"):
            return
        self.closing = True


def main():
    root = tk.Tk(); Studio(root); root.mainloop()


if __name__ == "__main__":
    main()
