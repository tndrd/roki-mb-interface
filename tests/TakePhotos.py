from picamera2 import Picamera2
import RokiPyTest as rpt
import Roki
import cv2
import time

TEST_TIME_S = 1
FRAME_DURATION_US = 16700
FRAME_DURATION_MS = FRAME_DURATION_US // 1000
DURATION_THRESHOLD = 4

picam2 = Picamera2(camera_num=0)

mb = Roki.Motherboard()

rpt.call(mb, Roki.MbDefaultConfig(mb))
rpt.call(mb, mb.ResetStrobeContainers())
rpt.call(mb, mb.ConfigureStrobeFilter(FRAME_DURATION_MS, DURATION_THRESHOLD))

picam2.start()
rpt.eprint(f"Running for {TEST_TIME_S}s...")
time.sleep(1)
picam2.set_controls({"FrameDurationLimits": (FRAME_DURATION_US, FRAME_DURATION_US)})
picam2.stop()

imu = rpt.call(mb, mb.GetIMUContainerInfo())
body = rpt.call(mb, mb.GetBodyContainerInfo())

if (body.NumAv == 0):
    rpt.failure_stop("No frames in body frCont")

if (imu.NumAv == 0):
    rpt.failure_stop("No frames in IMU frCont")

rpt.end_test()