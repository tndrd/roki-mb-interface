import sys
import os
from picamera2 import Picamera2

def mute_picamera():
    Picamera2.set_logging(Picamera2.ERROR)
    os.environ["LIBCAMERA_LOG_LEVELS"] = "4"

def eprint(msg, **kwargs):
    print(msg, **kwargs, file=sys.stderr)

def failure_stop(msg):
    eprint(msg)
    sys.exit(1)

def end_test():
    sys.exit(0)

def call(mb, val):
    ret = None

    if type(val) is tuple:
        ret = val[1]
        val = val[0]
    
    if val == False:
        failure_stop(mb.GetError())

    return ret