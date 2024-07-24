import RokiPyTest as rpt
from RokiPyTest import Picamera2
import Roki
import time
rpt.mute_picamera()

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
time.sleep(TEST_TIME_S)
picam2.stop()

imu = rpt.call(mb, mb.GetIMUContainerInfo())
body = rpt.call(mb, mb.GetBodyContainerInfo())

if rpt.check(body.NumAv == 0):
    rpt.failure_stop("No frames in body frCont")

if rpt.check(imu.NumAv == 0):
    rpt.failure_stop("No frames in IMU frCont")

rpt.call(mb, mb.ResetStrobeContainers())

imu = rpt.call(mb, mb.GetIMUContainerInfo())
body = rpt.call(mb, mb.GetBodyContainerInfo())

if rpt.check((body.NumAv != 0) or (body.First != 0)):
    rpt.failure_stop("Body frCont did not reset")

if rpt.check((imu.NumAv != 0) or (imu.First != 0)):
    rpt.failure_stop("IMU frCont did not reset")

rpt.end_test()