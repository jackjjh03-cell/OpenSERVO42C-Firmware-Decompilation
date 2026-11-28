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
from .tuning_trials import PROFILES, rank_trials
from .trial_capture import begin_trial, finish_trial
from .sweep import Sweep
from .lut_transfer import LutTransfer

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
        trials = ttk.Frame(tabs, padding=12); tabs.add(trials, text="PID trials")
        self.trials = []; self.trial_pending = None
        self.trial_end = self.field(trials, 0, "Test endpoint (absolute counts)", "")
        self.trial_count = self.field(trials, 1, "Samples (1–160)", "160")
        self.trial_interval = self.field(trials, 2, "Sample interval (ms)", "10")
        self.trial_tolerance = self.field(trials, 3, "Settling tolerance (counts)", "16")
        self.trial_tail = self.field(trials, 4, "Post-move observation (ms)", "200")
        self.trial_profile = tk.StringVar(value="Balanced")
        ttk.Label(trials,text="Ranking objective").grid(row=5,column=0,sticky="w")
        profiles=ttk.Combobox(trials,textvariable=self.trial_profile,values=PROFILES,state="readonly",width=18)
        profiles.grid(row=5,column=1,sticky="w"); profiles.bind("<<ComboboxSelected>>",lambda _:self.show_trials())
        ttk.Button(trials,text="Record one test move",command=self.start_trial).grid(row=6,column=0,sticky="w",pady=5)
        ttk.Button(trials,text="Retrieve completed trial",command=self.retrieve_trial).grid(row=6,column=1,sticky="w")
        ttk.Button(trials,text="Copy best gains to PID editor",command=self.copy_best_gains).grid(row=7,column=0,sticky="w")
        ttk.Button(trials,text="Clear trial comparisons",command=self.clear_trials).grid(row=7,column=1,sticky="w")
        self.trial_text = tk.StringVar(value="Arm position mode at a known starting point with travel limits and protect policy enabled. Record one test move, wait for the trace to finish, disable output, then retrieve. Return to the same start and repeat with other gains. Low effort is not a noise measurement.")
        ttk.Label(trials,textvariable=self.trial_text,justify="left",wraplength=850).grid(row=8,column=0,columnspan=3,sticky="w",pady=12)
        calibrate = ttk.Frame(tabs, padding=12); tabs.add(calibrate,text="Calibration")
        self.cal_active=False; self.cal_need_step=False; self.cal_sweep=None
        self.lut_active=False; self.lut_transfer=None
        self.cal_current=self.field(calibrate,0,"Calibration current (raw counts)","")
        self.cal_hysteresis=self.field(calibrate,1,"Max raw-count disagreement", "16")
        self.cal_text=tk.StringVar(value="Uncouple the motor from the mechanism. Configure and save drive settings, reboot, enable external EN, and keep motor output disabled. A calibration sweep invalidates the mechanical reference. No flash is written.")
        ttk.Button(calibrate,text="Start two-direction sweep",command=self.start_calibration).grid(row=2,column=0,sticky="w",pady=6)
        ttk.Button(calibrate,text="Stop calibration",command=self.stop_calibration).grid(row=2,column=1,sticky="w")
        ttk.Button(calibrate,text="Export generated LUT",command=self.export_calibration).grid(row=3,column=0,sticky="w")
        ttk.Button(calibrate,text="Program generated LUT",command=self.start_lut).grid(row=3,column=1,sticky="w")
        ttk.Button(calibrate,text="Abort LUT programming",command=self.abort_lut).grid(row=4,column=0,sticky="w")
        self.lut_progress=ttk.Progressbar(calibrate,maximum=32768,length=280)
        self.lut_progress.grid(row=4,column=1,sticky="w")
        ttk.Label(calibrate,textvariable=self.cal_text,justify="left",wraplength=850).grid(row=5,column=0,columnspan=3,sticky="w",pady=12)
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
        self.cal_active=False;self.cal_need_step=False;self.lut_active=False
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
        self.trials.clear(); self.trial_pending = None
        self.cal_active=False;self.cal_need_step=False;self.cal_sweep=None
        self.lut_active=False;self.lut_transfer=None;self.lut_progress["value"]=0
        self.trial_text.set('No comparison trials for this connection.')
        self.message.set("Connected. No motor commands sent.")

    def disconnect(self):
        if self.lut_active:
            self.abort_lut();return
        if self.cal_active:
            self.stop_calibration();self.message.set("Calibration stop requested; verify output before disconnecting.");return
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
                if self.lut_active:
                    self.lut_active=False
                    self.cal_text.set("LUT transfer interrupted: "+str(exc)+". Calibration may be invalid; inspect and restart from the beginning. No write retried.")
                    self.pending=(lambda c:c.write(p.LUT_ABORT),None,True)
                if self.cal_active:
                    self.cal_active=False;self.cal_need_step=False
                    self.cal_text.set("Calibration interrupted: "+str(exc)+". Verify output independently.")
                    self.pending=(lambda c:c.write(p.CAL_STOP),None,True)
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
        elif self.connected and self.future is None and self.lut_active:
            transfer=self.lut_transfer
            self.submit(lambda c:transfer.advance(c),self.lut_advance,True)
        elif self.connected and self.future is None and time.monotonic() >= self.next_poll:
            self.next_poll = time.monotonic() + 0.25
            if self.cal_active:
                if self.cal_need_step:
                    self.cal_need_step=False
                    self.submit(lambda c:c.write(p.CAL_STEP),quiet=True)
                else:
                    self.submit(lambda c:c.read(p.CAL_STATUS),self.calibration_sample,True)
            else:
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

    def start_calibration(self):
        if not self.connected or self.cal_active or self.lut_active or self.pending is not None or self.future is not None and not self.quiet:
            self.message.set("Connect and wait for the current operation.");return
        try:
            current=int(self.cal_current.get()); sweep=Sweep(int(self.cal_hysteresis.get()))
            if not 1<=current<=2047: raise ValueError("Enter a positive calibration current within the configured limit.")
        except ValueError as exc:
            self.message.set(str(exc));return
        if not messagebox.askyesno("Calibration motion","Motor will move one full turn forward and back. Confirm motor is physically uncoupled, limits disabled, current is appropriate, and an independent hardware stop is available."):
            return
        self.cal_sweep=sweep
        self.cal_active=True;self.cal_need_step=False
        self.cal_text.set("Starting calibration. Verify the shaft is free to turn.")
        self.submit(lambda c:c.write(p.CAL_START,current),quiet=False)

    def calibration_sample(self,status):
        if not self.cal_active: return
        self.cal_need_step=self.cal_sweep.accept(status)
        if self.cal_sweep.lut is not None:
            self.cal_active=False
            self.cal_text.set("Sweep complete; output disabled. Maximum disagreement: {} raw counts. Export or program the generated LUT. Board flash has not been changed.".format(self.cal_sweep.peak))
        else:
            self.cal_text.set("Collecting calibration knot {} / 400. Motor energized; use independent hardware stop if necessary.".format(status[0]))

    def stop_calibration(self):
        self.cal_active=False;self.cal_need_step=False
        self.submit(lambda c:c.write(p.CAL_STOP),urgent=True)
        self.cal_text.set("Stop requested. Confirm output is disabled before handling the motor.")

    def export_calibration(self):
        if self.cal_sweep is None or self.cal_sweep.lut is None:
            self.message.set("Complete a valid sweep first.");return
        path=filedialog.asksaveasfilename(defaultextension=".bin",filetypes=[("Raw LUT binary","*.bin")])
        if path:
            try:
                with open(path,"wb") as f:f.write(self.cal_sweep.lut)
            except OSError as exc:self.message.set(str(exc));return
            self.message.set("Calibration LUT exported. Not programmed to the controller.")

    def start_lut(self):
        if not self.connected or self.cal_active or self.lut_active or self.pending is not None or self.future is not None and not self.quiet:
            self.message.set("Connect and wait for the current operation.");return
        if self.cal_sweep is None or self.cal_sweep.lut is None:
            self.message.set("Complete a valid sweep on this controller first.");return
        if not messagebox.askyesno("Replace calibration LUT","This replaces the board-specific lookup table. Keep a full original flash backup and recovery programmer. Disable external EN, keep the motor uncoupled and do not remove power. Restore the original backup before returning to factory or older firmware. Continue?"):
            return
        self.lut_transfer=LutTransfer(self.cal_sweep.lut);self.lut_active=True
        self.lut_progress["value"]=0
        self.cal_text.set("Programming calibration. Output must remain disabled.")

    def lut_advance(self,result):
        if not self.lut_active:return
        count,done=result;self.lut_progress["value"]=count
        if done:
            self.lut_active=False
            self.cal_text.set("Calibration programmed and verified. Reboot the controller with EN inactive to validate the new table. Mechanical reference is invalid until confirmed.")
        else:
            self.cal_text.set("Programming LUT: {} / 32768 bytes acknowledged. Keep power on and EN inactive.".format(count))

    def abort_lut(self):
        self.lut_active=False
        self.submit(lambda c:c.write(p.LUT_ABORT),urgent=True)
        self.cal_text.set("Abort requested. A partial table remains invalid; restart programming from the beginning.")

    def show_calibration(self, data):
        winding, jump, forward, reverse, flat, flags = data
        self.calibration_text.set("Structural check: {}\nWinding: {} counts\nLargest adjacent change: {} counts\nForward: {}  Reverse: {}  Flat: {}\nMarkers: {}  Half-turn ambiguity: {}\nRead-only check; does not establish physical calibration accuracy.".format("passed" if flags == 3 else "failed",winding,jump,forward,reverse,flat,"valid" if flags&1 else "invalid","yes" if flags&4 else "no"))

    def start_trial(self):
        if not self.connected or self.future is not None and not self.quiet:
            self.message.set("Connect and wait for the current transaction."); return
        if self.trial_pending is not None:
            self.message.set("Retrieve or clear the previous trial first."); return
        try:
            args=tuple(int(v.get()) for v in (self.trial_end,self.trial_count,self.trial_interval,self.trial_tolerance,self.trial_tail))
        except ValueError:
            self.message.set("Enter integer trial settings, including a deliberate endpoint."); return
        if not messagebox.askyesno("Test move","Command one position move to {} counts with the currently applied gains? Verify clearance and the independent hardware stop.".format(args[0])):
            return
        def started(capture):
            self.trial_pending=capture
            self.trial_text.set("Test move accepted. Wait for trace completion, disable output, then retrieve. No gain changes have been sent.")
        self.submit(lambda c:begin_trial(c,*args),started)

    def retrieve_trial(self):
        capture=self.trial_pending
        if capture is None:
            self.message.set("Record a test move first."); return
        def retrieved(trial):
            try:
                rank_trials(self.trials+[trial],self.trial_profile.get())
            except ValueError as exc:
                self.trial_text.set(str(exc)+". Clear comparisons to start a new group."); return
            self.trials.append(trial);self.trial_pending=None;self.show_trials()
        self.submit(lambda c:finish_trial(c,capture),retrieved)

    def show_trials(self):
        if not self.trials:
            self.trial_text.set("No eligible trials recorded."); return
        ranking=rank_trials(self.trials,self.trial_profile.get())
        lines=["{} eligible trial(s) · {} objective".format(len(self.trials),self.trial_profile.get())]
        for i,(score,gains,count) in enumerate(ranking[:6]):
            lines.append("{}: Kp={} Ki={} Kd={} · score {:.3f} · {} run(s)".format(i+1,*gains[:3],score,count))
        lines.append("Lower score is better. Compare only trials on the same mechanism and load. Recommendation is not a stability guarantee.")
        self.trial_text.set("\n".join(lines))

    def copy_best_gains(self):
        if len({t.gains for t in self.trials})<2:
            self.message.set("Record at least two distinct gain configurations first."); return
        _,gains,_=rank_trials(self.trials,self.trial_profile.get())[0]
        self.fill(self.config_vars[1],gains)
        self.message.set("Best measured gains copied to PID editor only. Review before applying.")

    def clear_trials(self):
        if messagebox.askyesno("Clear comparisons","Clear recorded comparisons and the pending trial reference? This does not stop the motor or recording."):
            self.trials.clear();self.trial_pending=None;self.show_trials()

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
        if self.cal_active:self.stop_calibration()
        if self.lut_active:self.abort_lut()
        self.closing = True


def main():
    root = tk.Tk(); Studio(root); root.mainloop()


if __name__ == "__main__":
    main()
