import Roki
import RokiPyTest as rpt
from threading import Thread
import time

REMAIN_TOLERANCE = 5

def return_servo(rcb):
    sd = Roki.Rcb4.ServoData()
    sd.Id = 8
    sd.Sio = 0
    sd.Data = 7500
    rpt.call(rcb, rcb.setServoPos([sd], 5))

def fill_bq(rcb, count, pause):
    sd = Roki.Rcb4.ServoData()
    sd.Id = 8
    sd.Sio = 0
    
    do_pause = False
    for x in range(count):
        value = 7500 + (-0.5 + x / count) * 1000
        sd.Data = int(value)
        pause_val = 0
        if do_pause:
            pause_val = pause
            do_pause = False
        else:
            do_pause = True
        rcb.setServoPosAsync([sd], 5, pause_val)

def test_bq_period(period, pause = 0, TestReset = False):
    period_old = period
    period = int(period * (92 / 100))

    mb = Roki.Motherboard()
    rpt.call(mb, Roki.MbDefaultConfig(mb))

    rcb = Roki.Rcb4(mb)
    rpt.call(rcb, rcb.checkAcknowledge())

    count = rpt.call(mb, mb.GetBodyQueueInfo()).Capacity
    rpt.call(mb, mb.SetBodyQueuePeriod(period))

    queue_time = count * (period_old / 1000) * (pause / 2 + 1) 
    
    start = time.perf_counter()
    fill_bq(rcb, count, pause)
    filled = time.perf_counter()
    
    if rpt.is_mocking():
        sleep_time = 0
    else:
        sleep_time = queue_time - (filled - start)
    
    if TestReset:
        time.sleep(sleep_time / 2)
        rpt.call(mb, mb.ResetBodyQueue())
    else:
        time.sleep(sleep_time)

    remains = rpt.call(mb, mb.GetBodyQueueInfo()).Size

    if (remains > REMAIN_TOLERANCE):
        return_servo(rcb)
        rpt.failure_stop(f"BQ Period Error: remains {remains} objects")
    return_servo(rcb)

test_bq_period(5, 5)
test_bq_period(5, 0, True)
test_bq_period(10, 1)
test_bq_period(20, 1)
test_bq_period(20, 0, True)
test_bq_period(2)

rpt.end_test()
