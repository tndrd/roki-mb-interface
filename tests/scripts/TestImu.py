import sys
sys.path.append('/home/tndrd/starkit/kondo_2023/roki-mb-interface/build')

import Roki

mb = Roki.Motherboard()

conf = Roki.TTYConfig()

conf.Port = "/dev/ttyAMA2"
conf.Baudrate = 1250000
conf.Stopbits = Roki.Stopbits.One
conf.ParityBit = True
conf.Timeout = 2

mb.Configure(conf)

while True:
    fr = mb.GetCurrentOrientation()

    print("Quaternion: ")
    print("  X:  " + fr.Orientation.X)
    print("  Y:  " + fr.Orientation.Y)
    print("  Z:  " + fr.Orientation.Z)
    print("  W:  " + fr.Orientation.W)
    print("Timestamp: ")
    print("  S:  " + fr.Timestamp.TimeS)
    print("  NS: " + fr.Timestamp.TimeNS)
    print("SensorID: " + fr.SensorID)
    print("\n")

