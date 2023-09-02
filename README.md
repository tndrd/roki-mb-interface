# Humaniod robot "Roki" - Motherboard MCU communication library 
Library provides communication interface between "Roki"s RaspberryPi and MCU unit embedded in motherboard PCB.

# Simple installation
```bash
git clone https://github.com/tndrd/roki-mb-interface.git
cd roki-mb-interface
sudo sh install.sh
```

# Build process
## Install dependencies
```bash
sudo apt install cmake
```
## Build
```bash
git clone https://github.com/tndrd/roki-mb-interface.git --recursive
cd roki-mb-interface
mkdir build
cd build
cmake ..
cmake --build .
```
## Install python library
```bash
cmake --install .
```
> Cmake installs the python module into Python site-packages directory.


# Usage (Python)
After installation, the module can be simply included from any directory.

```python
import Roki
```

## Important notice
It is crucial to properly create the module's objects. Otherwise some functions may not accept these objects as an arguments. Notice that empty brackets are used to call the default constructor: 
```
mb = Roki.Motherboard() # OK
mb = Roki.Motherboard   # Not OK
```
This principle applies to any of the library's objects, including ```Roki.Rcb4.ServoData()```.

## Motherboard interface configuration
Class ```Motherboard``` is used to communicate with Roki motherboard. Before use, it needs to be configured. This example shows how to configure it with default Roki parameters:
```python
mb = Roki.Motherboard()
conf = Roki.TTYConfig()

conf.Port = "/dev/ttyAMA2"
conf.Baudrate = 1250000
conf.Stopbits = Roki.Stopbits.One
conf.ParityBit = True
conf.Timeout = 2  # seconds

mb.Configure(conf)
```

After the successful configuration you can start using it.

> This class IS NOT thread-safe in current version. Please do not use this class from separate threads.

> In current version ```Motherboard``` class does not make acknowlege with motherboard's MCU. If connection is not estabilished, an error will appear only after first ```Motherboard``` method call. 

## IMU Communication
### Current orientation
You can get the latest orientation data via ```Motherboard.GetCurrentOrientation()``` method:
```python
fr = mb.GetCurrentOrientation() # Return type is Motherboard.IMUFrame

print("Quaternion: ")
print("  X:  " + str(fr.Orientation.X))
print("  Y:  " + str(fr.Orientation.Y))
print("  Z:  " + str(fr.Orientation.Z))
print("  W:  " + str(fr.Orientation.W))
print("Timestamp: ")
print("  S:  " + str(fr.Timestamp.TimeS))
print("  NS: " + str(fr.Timestamp.TimeNS))
print("SensorID: " + str(fr.SensorID))
```
### The frame container
The motherboard is connected to camera's STROBE pin. After camera takes a shot it sends a pulse to the notherboard MCU. When the pulse is recieved, MCU gives the latest frame a sequential number [Seq] and stores the frame in a container. The container's capacity is limited, so at some point older frames will be erased.

### Get container info
You can check the contents of the frame container via ```Motherboard.GetIMUInfo()``` method:
```python
info = mb.GetIMUInfo()

print("Imu info: ")
print("  First: " + str(info.First)) # Oldest available frame's seq number
print("  NumAv: " + str(info.NumAv)) # How many frames are available right now
print("  MaxFr: " + str(info.MaxFrames)) # The container's max capacity
```

For example, if ```info.First == 42``` and ```info.NumAv == 100```, then all the frames with seq numbers from ```42``` to ```141``` will be available.

### Clear the container
To clear the frame container and reset the frame counter, you should use ```Motherboard.ResetIMUCounter()``` method:

```python
mb.ResetIMUCounter()
```

### Get frame by sequential number
You can get the frame by its sequential number via ```Motherboard.GetOrientationBySeq()``` method.

```python
fr = mb.GetOrientationBySeq(42) # Check the availability first 

print("Quaternion: ")
print("  X:  " + str(fr.Orientation.X))
print("  Y:  " + str(fr.Orientation.Y))
print("  Z:  " + str(fr.Orientation.Z))
print("  W:  " + str(fr.Orientation.W))
print("Timestamp: ")
print("  S:  " + str(fr.Timestamp.TimeS))
print("  NS: " + str(fr.Timestamp.TimeNS))
print("SensorID: " + str(fr.SensorID))
```

If the frame is not available right now, an exception will be thrown. So please check the availability of desired frame before the request.

## Body communication
### Rcb4 interface
In current version body and motherboard communication protocol is Rcb4. It is incapsulated in ```Roki.Rcb4``` class. This class refers to ```Roki.Motherboard``` and uses its internal functions.

```python
mb = Roki.Motherboard()
conf = Roki.TTYConfig()
...
mb.Configure(conf)

rcb = Roki.Rcb4(mb)
```

> Because ```Roki.Motherboard``` is not thread-safe in current version, the ```Roki.Rcb4``` is not thread-safe too. Do not use them in separate threads.
### Rcb4 usage
```Roki.Rcb4``` provides Rcb4's protocol commands. For example, let's use make handshake with body controller via ```Rcb4.checkAcknowledge()```: 
```python
print("Connecting to body...")
if(not rcb.checkAcknowledge()):
    print(rcb.getError())
    quit()
```
> By default, ```Roki.Rcb4``` does not throw exceptions. You should check the return value if you want to know if operation has succeeded. If the return value is false, you can get error description via ```Rcb4.getError()```

### Synchronous commands
Servo position is set via ```Rcb4.setServoPos```:
```python
sd = Roki.Rcb4.ServoData()
sd.Id = 8
sd.Sio = 1
sd.Data = 7500

if (not rcb.setServoPos([sd], 5)):
    print(rcb.GetError())
    quit()
```
### Body queue
The motherboard accepts asychronous type of commands. When the async command is recieved by motherboard MCU, it is stored in queue. Every 20 milliseconds MCU sends one command to the body (if one exists). The responce is not forwarded back to RaspberryPi.

> The main goal of the body queue is to smoothen up the conistency of intervals between each command's transmission to body controller.

> The queue is transparent for synchronous commands.

### Asynchronous commands
For example, we can send 500 positions to the queue via ```Rcb4.setServoPosAsync()```:

```python
from math import sin
...

sd = Roki.Rcb4.ServoData()
sd.Id = 8
sd.Sio = 1

x = 0
while x < 500:
    sd.Data = 7500 + int(500 * sin(0.1 * x))
    if (not rcb.setServoPosAsync([sd], int(5))):
        print(rcb.getError())
        quit()
    x = x + 1
```

### Body queue state
The state of the body queue can be checked via ```Motherboard.getQueueInfo()```:
```python
info = mb.GetQueueInfo()                    # How many requests are now
print("Requests: " + str(info.NumRequests)) # in queue
```