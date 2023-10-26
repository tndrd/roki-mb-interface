import Roki
import RokiPyTest as rpt
from threading import Thread
import time

REMAIN_TOLERANCE = 2

def fill_bq(rcb, count):
    sd = Roki.ServoData()
    sd.Id = 8
    sd.Sio = 0
    
    for x in range(count):
        value = 7500 + (-0.5 + x / count) * 1000
        sd.Value = value
        rcb.SetServoPosAsync([sd], 5)

def test_bq_period(period):
    mb = Roki.Motherboard()
    rpt.call(mb, Roki.MbDefaultConfig(mb))

    rcb = Roki.Rcb4(mb)
    rpt.call(rcb, rcb.checkAcknowledge())

    count = rpt.call(mb, mb.GetBodyQueueInfo()).Capacity
    rpt.call(rcb, rcb.SetBodyQueuePeriod(period))

    thrd = Thread(target = fill_bq, args=[count])
    thrd.run()

    time.sleep(count * (period / 1000))

    remains = rpt.call(mb, mb.GetBodyQueueInfo()).Size

    if (remains > REMAIN_TOLERANCE):
        rpt.failure_stop(f"BQ Period Error: remains {remains} objects")

    thrd.join()

test_bq_period(5)
test_bq_period(10)
test_bq_period(20)
#test_bq_period(100)

rpt.end_test()