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
info = mb.GetIMUInfo()

print(f"Found {info.NumAv} frames, starting from {info.First}: ")

for i in range(info.First, info.First + info.NumAv):
    fr = mb.GetStrobeFrame(i).Orientation

    print(f"Quaternion: seq #{i}")
    print("  X:  " + str(fr.Orientation.X))
    print("  Y:  " + str(fr.Orientation.Y))
    print("  Z:  " + str(fr.Orientation.Z))
    print("  W:  " + str(fr.Orientation.W))
    print("Timestamp: ")
    print("  S:  " + str(fr.Timestamp.TimeS))
    print("  NS: " + str(fr.Timestamp.TimeNS))
    print("SensorID: " + str(fr.SensorID))
    print("\n")

