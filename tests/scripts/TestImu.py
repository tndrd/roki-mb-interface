import sys
import time

import Roki

mb = Roki.Motherboard()

conf = Roki.TTYConfig()

conf.Port = "/dev/ttyAMA2"
conf.Baudrate = 1250000
conf.Stopbits = Roki.Stopbits.One
conf.ParityBit = True
conf.Timeout = 2

mb.Configure(conf)
#mb.ResetIMUCounter()

while True:
    fr = mb.GetCurrentOrientation()
    #fr = mb.GetOrientationBySeq(150)

    print("Quaternion: ")
    print("  X:  " + str(fr.Orientation.X))
    print("  Y:  " + str(fr.Orientation.Y))
    print("  Z:  " + str(fr.Orientation.Z))
    print("  W:  " + str(fr.Orientation.W))
    print("Timestamp: ")
    print("  S:  " + str(fr.Timestamp.TimeS))
    print("  NS: " + str(fr.Timestamp.TimeNS))
    print("SensorID: " + str(fr.SensorID))
    print("\n")

    info = mb.GetIMUInfo()

    print("Imu info: ")
    print("  First: " + str(info.First))
    print("  NumAv: " + str(info.NumAv))
    print("  MaxFr: " + str(info.MaxFrames))
    print("\n")

    time.sleep(0.5)

