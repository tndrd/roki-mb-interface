import sys

def eprint(msg):
    print(msg, file=sys.stderr)

def failure_stop(msg):
    eprint(msg)
    sys.exit(1)

def end_test():
    eprint("OK")
    sys.exit(0)

def call(mb, val):
    ret = 0
    
    if type(val) == type(tuple):
        ret = val[1]
        val = val[0]
    
    if val == False:
        failure_stop(mb.GetError())

    return ret