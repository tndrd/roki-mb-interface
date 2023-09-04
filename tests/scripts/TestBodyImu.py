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
#mb.ResetIMUCounter()

sd = Roki.Rcb4.ServoData()
sd.Id = 4
sd.Sio = 1

# x = 0
# req = 0
# while x < 500:
#     sd.Data = 7500 + int(800 * sin(0.1 * x))
#     rcb.setServoPosAsync([sd], int(5))
#     req = mb.GetQueueInfo().NumRequests
#     print(req)
#     x = x + 1
#     
# sd.Sio = 2
# sd.Data = 7500 + 1000
# sd1 = Roki.Rcb4.ServoData()
# sd1.Id = 3
# sd1.Sio = 2
# sd1.Data = 7500 - 1000
# rcb.setServoPos([sd, sd1], int(2))

# while req > 0:
#     print(rcb.checkAcknowledge())
#     req = mb.GetQueueInfo().NumRequests
#     print(req)

# cycles = 100
# start0 = time.perf_counter()
# for _ in range(cycles):
#     #print('sending')
#     result, servo_pos_bytes = rcb.moveRamToComCmdSynchronize(0x0070, 52)
#     #print(result, bytes(servo_pos_bytes))
#     try:
#         servo_pos = struct.unpack('<hhhhhhhhhhhhhhhhhhhhhhhhhh', bytes(servo_pos_bytes))
#     except Exception: continue
#     #print(clock.fps())
#     #print(servo_pos)
#     #time.sleep(0.2)
#     
# print('fps : ', cycles/(time.perf_counter() - start0))

cycles = 10000
start0 = time.perf_counter()
for _ in range(cycles):
    result, quat_bytes = rcb.moveRamToComCmdSynchronize(0x0060, 8)
    
    if (not result):
      print(rcb.getError())
      continue

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

