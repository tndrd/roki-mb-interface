import Roki
import RokiPyTest as rpt

frame = Roki.IMUFrame()

_ = frame.Orientation.X
_ = frame.Orientation.Y
_ = frame.Orientation.Z
_ = frame.Orientation.W

_ = frame.Timestamp.TimeS
_ = frame.Timestamp.TimeNS
_ = frame.SensorID

info = Roki.FrameContainerInfo()
_ = info.First
_ = info.NumAv
_ = info.MaxFrames

info = Roki.BodyQueueInfo()
_ = info.Size
_ = info.Capacity

_ = Roki.Stopbits.One
_ = Roki.Stopbits.Two

conf = Roki.TTYConfig()
_ = conf.Port
_ = conf.Baudrate
_ = conf.Stopbits
_ = conf.ParityBit
_ = conf.Timeout


mb = Roki.Motherboard()

mb.Configure(conf)
mb.GetIMUFrame(0)
mb.GetBodyFrame(0)
mb.GetIMUContainerInfo()
mb.GetBodyContainerInfo()
mb.ResetStrobeContainers()
mb.SetIMUStrobeOffset(0)
mb.SetBodyStrobeOffset(0)
mb.GetIMULatest()
mb.GetStrobeWidth()
mb.ConfigureStrobeFilter(0,0)
mb.GetBodyQueueInfo()
mb.SetBodyQueuePeriod(0)
mb.IsOk()
mb.GetError()
mb.ResetBodyQueue()

Roki.MbDefaultConfig(mb)

sd = Roki.Rcb4.ServoData()
_ = sd.Id
_ = sd.Sio
_ = sd.Data

rcb = Roki.Rcb4(mb)
rcb.checkAcknowledge()
rcb.setServoPos([], 0)

rcb.setServoPosAsync([], 0)
rcb.setServoPosAsync([], 0, 42)
rcb.moveRamToComCmdSynchronize(0x0060, 8)
rcb.motionPlay(0)
rcb.GetError()
rcb.IsOk()

zubr = Roki.Zubr(mb)
zubr.memISet(0, 0)
zubr.memFSet(0, 0)
zubr.memIGet(0)
zubr.memFGet(0)

rpt.end_test()