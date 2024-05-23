import tkinter as tk
from tkinter import ttk, messagebox
from threading import Thread
import subprocess
import sys
import os

class BallTrackingApp:
    def __init__(self, master):
        self.master = master
        master.title("Ball Tracking")

        self.create_widgets()
        self.tracking_thread = None
        self.tracking = False

    def create_widgets(self):
        self.label_video = ttk.Label(self.master, text="Video Source (URL or Path):")
        self.label_video.pack(pady=5)

        self.entry_video = ttk.Entry(self.master, width=50)
        self.entry_video.pack(pady=5)

        self.label_buffer = ttk.Label(self.master, text="Buffer:")
        self.label_buffer.pack(pady=5)

        self.entry_buffer = ttk.Entry(self.master, width=10)
        self.entry_buffer.insert(0, "64")
        self.entry_buffer.pack(pady=5)

        self.start_button = ttk.Button(self.master, text="Start Tracking", command=self.start_tracking)
        self.start_button.pack(pady=5)

        self.stop_button = ttk.Button(self.master, text="Stop Tracking", command=self.stop_tracking)
        self.stop_button.pack(pady=5)

        self.quit_button = ttk.Button(self.master, text="Quit", command=self.quit)
        self.quit_button.pack(pady=5)

    def start_tracking(self):
        video_source = self.entry_video.get()
        buffer_size = self.entry_buffer.get()

        if not buffer_size.isdigit():
            messagebox.showerror("Invalid Input", "Buffer size must be a number.")
            return

        buffer_size = int(buffer_size)
        self.tracking = True
        self.tracking_thread = Thread(target=self.run_tracking, args=(video_source, buffer_size))
        self.tracking_thread.start()

    def stop_tracking(self):
        self.tracking = False
        if self.tracking_thread:
            self.tracking_thread.join()
            self.tracking_thread = None

    def run_tracking(self, video_source, buffer_size):
        command = [sys.executable, "ball_tracking.py", "--buffer", str(buffer_size)]
        
        if video_source:
            command.extend(["--video", video_source])
        
        process = subprocess.Popen(command)
        process.communicate()

    def quit(self):
        self.stop_tracking()
        self.master.quit()

def main():
    root = tk.Tk()
    app = BallTrackingApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()
