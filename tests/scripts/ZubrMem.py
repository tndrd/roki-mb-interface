import Roki
import RokiPyTest as rpt

ADDR = 600

VALUE_INT = int(2**31 -1)
VALUE_FLOAT = 0.424242

FLOAT_TOLERANCE = 10**-5

mb = Roki.Motherboard()
rpt.call(mb, Roki.MbDefaultConfig(mb))

zubr = Roki.Zubr(mb)

rpt.call(zubr, zubr.memISet(ADDR, VALUE_INT))

ret, val = zubr.memIGet(ADDR)

if not ret:
    rpt.failure_stop(f"Failed to read data: {zubr.GetError()}")

if rpt.check(val != VALUE_INT):
    rpt.failure_stop(f"Sent and recieved values are different")

rpt.call(zubr, zubr.memFSet(ADDR, VALUE_FLOAT))

ret, val = zubr.memFGet(ADDR)

if not ret:
    rpt.failure_stop(f"Failed to read data: {zubr.GetError()}")

if rpt.check(abs(val - VALUE_FLOAT) > FLOAT_TOLERANCE):
    rpt.failure_stop("Sent and received values are different")

rpt.end_test()
