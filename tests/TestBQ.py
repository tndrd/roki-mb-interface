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

def fill_bq(rcb, count):
    sd = Roki.Rcb4.ServoData()
    sd.Id = 8
    sd.Sio = 0
    
    for x in range(count):
        value = 7500 + (-0.5 + x / count) * 1000
        sd.Data = int(value)
        rcb.setServoPosAsync([sd], 5, 0)

def test_bq_period(period):
    period_old = period
    period = int(period * (92 / 100))

    mb = Roki.Motherboard()
    rpt.call(mb, Roki.MbDefaultConfig(mb))

    rcb = Roki.Rcb4(mb)
    rpt.call(rcb, rcb.checkAcknowledge())

    count = rpt.call(mb, mb.GetBodyQueueInfo()).Capacity
    rpt.call(mb, mb.SetBodyQueuePeriod(period))

    queue_time = count * (period_old / 1000) 
    
    start = time.perf_counter()
    fill_bq(rcb, count)
    filled = time.perf_counter()

    time.sleep(queue_time - (filled - start))

    remains = rpt.call(mb, mb.GetBodyQueueInfo()).Size

    if (remains > REMAIN_TOLERANCE):
        return_servo(rcb)
        rpt.failure_stop(f"BQ Period Error: remains {remains} objects")
    return_servo(rcb)

test_bq_period(20)
test_bq_period(10)
test_bq_period(20)
#test_bq_period(100)

rpt.end_test()