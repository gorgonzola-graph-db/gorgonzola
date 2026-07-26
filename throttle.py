import subprocess
import time
import sys

proc = subprocess.Popen(sys.argv[1:])
try:
    while proc.poll() is None:
        time.sleep(0.8)
        proc.send_signal(19) # SIGSTOP
        time.sleep(0.2)
        proc.send_signal(18) # SIGCONT
except KeyboardInterrupt:
    proc.terminate()
sys.exit(proc.returncode)
