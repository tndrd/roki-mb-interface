import sys
sys.path.append('/home/pi/Tndrd/roki-mb-interface/build')
import time

import Roki
from math import sin

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
sd.Id = 8
sd.Sio = 1

x = 0
req = 0
while x < 500:
    sd.Data = 7500 + int(500 * sin(0.1 * x))
    rcb.setServoPosAsync([sd], int(5))
    req = mb.GetQueueInfo().NumRequests
    print(req)
    x = x + 1

while req > 0:
    print(rcb.checkAcknowledge())
    req = mb.GetQueueInfo().NumRequests
    print(req)
    

