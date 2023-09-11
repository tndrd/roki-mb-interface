#import sys
#sys.path.append('/home/pi/roki-mb-interface/build')
import time
import struct
import math
import Roki
from math import sin

def quaternion_to_euler_angle(quaternion):
    euler_angle = {}
    w,x,y,z = quaternion
    ysqr = y*y
    t0 = +2.0 * (w * x + y * z)
    t1 = +1.0 - 2.0 * (x * x + ysqr)
    X = math.atan2(t0,t1)
    t2 = +2.0 * (w * y - z * x)
    t2 = +1.0 if t2 > +1.0 else t2
    t2 = -1.0 if t2 < -1.0 else t2
    Y = math.asin(t2)
    t3 = +2.0 * (w * z + x * y)
    t4 = +1.0 - 2.0 * (ysqr + z * z)
    Z = math.atan2(t3,t4)
    euler_angle['yaw'] = Z
    euler_angle['pitch'] = X
    euler_angle['roll'] = Y
    return euler_angle

mb = Roki.Motherboard()

conf = Roki.TTYConfig()

conf.Port = "/dev/ttyAMA2"
conf.Baudrate = 1250000
conf.Stopbits = Roki.Stopbits.One
conf.ParityBit = True
conf.Timeout = 2

mb.Configure(conf)

rcb = Roki.Rcb4(mb)

print("Connectiong to body...")
if(not rcb.checkAcknowledge()):
    print(rcb.getError())
    quit()

print("Ok")

sd = Roki.Rcb4.ServoData()
sd.Id = 4
sd.Sio = 1
sd.Data = 7500

doAsync = 0

while True:
    doAsync = (doAsync + 1) % 2
    cycles = 1000
    start0 = time.perf_counter()
    for _ in range(cycles):
        if doAsync == 0 :
            if not rcb.setServoPosAsync([sd], int(5)):
                print(rcb.getError())
                quit()
        
        result, quat_bytes = rcb.moveRamToComCmdSynchronize(0x0060, 8)
        if (not result):
            print(rcb.getError())
            quit()

        #if (quat_bytes[-1] != 42):
        #    print("Sequence Error")
        #    quit()

        try:
            quat = struct.unpack('<hhhh', bytes(quat_bytes))
        except Exception: continue
        x = quat[0]/16384
        y = quat[1]/16384
        z = quat[2]/16384
        w = quat[3]/16384
        #print('x = ' , x, 'y = ' , y, 'z = ' , z, 'w = ' , w)
        euler_angle = quaternion_to_euler_angle((w,x,y,z))
        euler_angle['pitch'] -= math.pi/2
        euler_angle['roll'] *= -1
        print('\b', euler_angle, '\t', quat_bytes)
    print('fps : ', cycles/(time.perf_counter() - start0))

