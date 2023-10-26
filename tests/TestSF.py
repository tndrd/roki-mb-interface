from picamera2 import Picamera2
import RokiPyTest as rpt
import Roki
import time

TEST_TIME_S = 1
UPDATE_PERIOD_S = 0.1
FRAME_DURATION_US = 16700
FRAME_DURATION_MS = FRAME_DURATION_US // 1000
DURATION_THRESHOLD = 4
FRAME_TOLERANCE = 2

n_frames = 0
n_strobes = 0

do_compare = True
mb = Roki.Motherboard()

def fits(val, target):
    return abs(val - target) < target * 0.02

def pre_callback(request):
    duration = request.get_metadata()['FrameDuration']
    if fits(duration, FRAME_DURATION_US):
        n_frames += 1
        if not do_compare: return

        cont = rpt.call(mb, mb.GetIMUContainerInfo())
        n_strobes = cont.First + cont.NumAv

        if (abs(n_frames - n_strobes) > FRAME_TOLERANCE):
            width = rpt.call(mb, mb.GetStrobeWidth)
            rpt.eprint(f"Strobe width: {width}")
            rpt.failure_stop(f"SFE[ got {n_strobes}, exp {n_frames}]")

picam2 = Picamera2(camera_num=0)
picam2.pre_callback = pre_callback

rpt.call(mb, Roki.MbDefaultConfig(mb))
rpt.call(mb, mb.ResetStrobeContainers())
rpt.call(mb, mb.ConfigureStrobeFilter(FRAME_DURATION_MS, DURATION_THRESHOLD))

picam2.start()
rpt.eprint(f"Running for {TEST_TIME_S}s...")
time.sleep(1)
picam2.set_controls({"FrameDurationLimits": (FRAME_DURATION_US, FRAME_DURATION_US)})

n_ticks = TEST_TIME_S // UPDATE_PERIOD_S
ticks = 0
while (ticks < n_ticks):
    rpt.eprint(f"\rs: {n_strobes}, f: {n_frames}")
    ticks += 1
    time.sleep(UPDATE_PERIOD_S)

picam2.stop()

rpt.call(mb, mb.ResetStrobeContainers())

imu = rpt.call(mb, mb.GetIMUContainerInfo())
body = rpt.call(mb, mb.GetBodyContainerInfo())

if (body.NumAv != 0):
    rpt.failure_stop("Body frCont did not reset")

if (imu.NumAv != 0):
    rpt.failure_stop("IMU frCont did not reset")

rpt.end_test()