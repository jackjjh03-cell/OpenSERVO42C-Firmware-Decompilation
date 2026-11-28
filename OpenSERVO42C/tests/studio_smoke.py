"""Graphical smoke test against the native C runtime fixture, under a display."""
import os
import subprocess
import sys
import tempfile
import time
import tkinter as tk
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from studio import protocol as p, preferences
from studio.app import Studio
from studio.client import ControllerClient


class Pipe:
    def __init__(self, executable):
        self.proc=subprocess.Popen([executable,'--run'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,text=True)
        self.fail=False
    def write(self, packet):
        if self.fail:raise OSError('Simulated cable removal')
        self.proc.stdin.write(packet.hex()+'\n');self.proc.stdin.flush();return len(packet)
    def read(self, size):return bytes.fromhex(self.proc.stdout.readline().strip())
    def close(self):
        self.proc.stdin.close();self.proc.wait(timeout=5);self.proc.stdout.close()


def main():
    with tempfile.TemporaryDirectory() as folder:
        root=tk.Tk();errors=[];ports=[]
        root.report_callback_exception=lambda *args:errors.append(args)
        def opener(device):
            port=Pipe(sys.argv[1]);ports.append(port);return ControllerClient(port)
        app=Studio(root,opener,Path(folder)/'prefs.json')
        def until(predicate):
            deadline=time.monotonic()+8
            while not predicate():
                root.update();time.sleep(.01)
                if errors:raise AssertionError(errors)
                if time.monotonic()>deadline:raise AssertionError('GUI transaction did not complete: '+app.message.get())
        try:
            root.update();app.connect();until(lambda:app.connected and app.snapshot is not None)
            assert app.connection_text.get()=='CONNECTED 0.1.1'
            app.command(p.SET_PID,2048,32,64,2,500);until(lambda:app.future is None and app.pending is None)
            app.submit(lambda c:c.read(p.GET_PID),lambda values:app.fill(app.config_vars[1],values))
            until(lambda:app.config_vars[1][0].get()=='2048')
            # Display every tab, and all four graph modes with live history.
            notebook=next(w for w in root.winfo_children() if w.winfo_class()=='TNotebook')
            for tab in notebook.tabs():notebook.select(tab);root.update()
            notebook.select(0);root.update()
            app.history.extend([(1,10,15,100,200),(2,13,18,-30,450)])
            app.graph_range.set('10 s')
            for view in preferences.VIEWS:
                app.graph_view.set(view);app.draw_graph();root.update()
                assert view in app.graph_caption.get()
            app.graph_paused.set(True);app.toggle_graph();assert app.frozen_history
            app.clear_history();assert not app.history and not app.graph_paused.get()
            ports[-1].fail=True;until(lambda:app.link_uncertain)
            assert not app.submit(lambda c:c.write(p.MOVE_ABS,10))
            assert app.connection_text.get()=='LINK LOST / UNKNOWN'
            # Disconnect without an interactive confirmation in the test.
            import studio.app as module
            module.messagebox.askyesno=lambda *args:True
            app.disconnect();until(lambda:not app.connected)
            assert app.connection_text.get()=='DISCONNECTED'
            app.connect();until(lambda:app.connected and app.snapshot is not None)
            app.close()
            deadline=time.monotonic()+5
            while time.monotonic()<deadline:
                root.update();time.sleep(.01)
                if not app.connected and app.future is None:break
            assert not errors,errors
            assert preferences.load(Path(folder)/'prefs.json')['graph_view']=='Load'
            print('Studio graphical/runtime smoke test passed')
        finally:
            app.worker.shutdown(wait=True)
            for port in ports:
                if port.proc.poll() is None:port.close()
            try:root.destroy()
            except tk.TclError:pass


if __name__=='__main__':main()
