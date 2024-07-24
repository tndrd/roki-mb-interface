import Roki
import RokiPyTest as rpt
import time
import struct

TESTING_TIME_S = 20
UPDATE_PERIOD_MS = 10
SUM_TOLERANCE = 0.2
ROUND_DIGITS = 2

mb = Roki.Motherboard()

rpt.call(mb, Roki.MbDefaultConfig(mb))
rcb = Roki.Rcb4(mb)
#rpt.call(rcb, rcb.checkAcknowledge())

rpt.eprint(f"Running for {TESTING_TIME_S}s...")

count = 0

for i in range(0, (1000 * TESTING_TIME_S) // UPDATE_PERIOD_MS):
    count = count + 1
    result, data = rcb.moveRamToComCmdSynchronize(0x0060, 8)

    if not result:
        rpt.failure_stop(f"Failed to obtain Body IMU data: {rcb.GetError()}")

    try: quat = struct.unpack("<hhhh", bytes(data))
    except Exception: rpt.failure_stop(f"Failed to unpack bytes: {bytes(data)}")

    norm = 16384

    x = quat[0] / norm
    y = quat[1] / norm
    z = quat[2] / norm
    w = quat[3] / norm

    s = x*x + y*y + z*z + w*w
    
    if rpt.check(abs(s - 1) > SUM_TOLERANCE):
        report = f"Bad quaternion sum: {round(s, ROUND_DIGITS)}\n"
        report += f"Bytes: {list(map(hex, data))}\n"
        report += f" x = {round(x, ROUND_DIGITS)}\n"
        report += f" y = {round(y, ROUND_DIGITS)}\n"
        report += f" z = {round(z, ROUND_DIGITS)}\n"
        report += f" w = {round(w, ROUND_DIGITS)}\n"
        print(count)

        result, data = rcb.moveRamToComCmdSynchronize(0x65, 8)

        print(result)
        print(list(map(hex, data)))

        rpt.failure_stop(report)

    time.sleep(UPDATE_PERIOD_MS / 1000)

rpt.end_test()

