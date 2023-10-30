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
```python
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

ok = mb.Configure(conf)
```

After the successful configuration you can start using it.

> This class IS NOT thread-safe in current version. Please do not use this class from separate threads.

## Error handling
All the library functions may produce errors in some cases. The library signalizes the error situation via return value. The first return value is always a boolean value (Success/Failure). The second return value provides the return data (if it exists). The content of return data is undefined in error cases (first argument is ```False```). Here are some examples:

```python
ok, frame = mb.GetImuLatest()   # ok is True => Success, we can read frame
ok = mb.ResetStrobeContainers() # ok is True => Success
```

If the function returned ```False``` as it's first return value, you can read the error description via ```Motherboard.GetError()```:

```python
ok = mb.ResetStrobeContainers()

if ok == False:
    print(mb.GetError())
```

> In further examples error checks will be skipped for better readability

## IMU Communication
### Current orientation
You can get the latest orientation data via ```Motherboard.GetIMULatest()``` method:
```python
ok, fr = mb.GetIMULatest() # Return type is Motherboard.IMUFrame

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

## Body communication
### Rcb4 interface
In current version body and motherboard communication protocol is Rcb4. It is incapsulated in ```Roki.Rcb4``` class. This class refers to ```Roki.Motherboard``` and uses its internal functions.

```python
mb = Roki.Motherboard()
conf = Roki.TTYConfig()
...
ok = mb.Configure(conf)

rcb = Roki.Rcb4(mb)
```

> Because ```Roki.Motherboard``` is not thread-safe in current version, the ```Roki.Rcb4``` is not thread-safe either. Do not use them in separate threads.

### Rcb4 usage and Error handling
```Roki.Rcb4``` provides Rcb4's protocol commands. For example, let's perform handshake with body controller via ```Rcb4.checkAcknowledge()```: 

```python
ok = rcb.checkAcknowledge():
if not ok:    
    print(rcb.GetError())
```

Similar to ```Motherboard``` class, ```Rcb4``` class provides the status as its first return value. The error description is accessed via ```Rcb4.GetError()```.

### Synchronous commands
Servo position can be set via ```Rcb4.setServoPos```:
```python
sd = Roki.Rcb4.ServoData()
sd.Id = 8
sd.Sio = 1
sd.Data = 7500

ok = rcb.setServoPos([sd], 5):
```

### Body queue
The motherboard accepts asychronous type of commands. When the async command is recieved by motherboard MCU, it is stored in queue. The queue has configurable ```period``` value. Each command sent to queue has its configurable ```pause``` value.

The first command received will be executed instantly. Then the body queue will sleep for one period. If the first's command ```pause``` is equal to zero the next available command will be executed. Otherwise, if ```pause``` is not zero, the queue will do nothing for ```pause``` periods.

Then the same process will repeat for each available command.

> The responces are not forwarded back to RaspberryPi.

> The main goal of the body queue is to smoothen up the conistency of intervals between each command's transmission to body controller.

> The queue is transparent for synchronous commands.

### Asynchronous commands
For example, we can send 500 positions to the queue via ```Rcb4.setServoPosAsync()```. Each package afterpause value will be set as ```3```:

```python
from math import sin
...

sd = Roki.Rcb4.ServoData()
sd.Id = 8
sd.Sio = 1

x = 0
while x < 500:
    sd.Data = 7500 + int(500 * sin(0.1 * x))
    rcb.setServoPosAsync([sd], int(5), 3):
    x = x + 1
```

> ```Rcb.setServoPosAsync()``` accepts ```pause``` argument as its third parameter. It is ```0``` by default.

### Body queue state
The state of the body queue can be accessed via ```Motherboard.GetBodyQueueInfo()```:

```python
ok, info = mb.GetBodyQueueInfo()
print("Requests: " + str(info.Size))     # Quantity of requests in queue
print("Capacity: " + str(info.Capacity)) # Maximum queue capacity
```

### Body queue configuration
The period of body queue can be set via ```Motherboard.SetBodyQueuePeriod()```:

```python
ok = mb.SetBodyQueuePeriod(25) # ms
```

The body queue can be cleared via ```Motherboard.ResetBodyQueue()```:
```python
ok = mb.ResetBodyQueue()
```

## Strobe Containers
The motherboard is connected to camera's STROBE pin. After camera takes a shot it sends a pulse to the motherboard MCU.

The motherboard has to filter the pulses. You can configure the filtration via configuring StrobeFilter entity (the process is described below).

The motherboard counts all the filtered pulses and gives each one a sequential number.

On each pulse motherboard collects the IMU data and all the servo positions and stores them in separate strobe containers.

The containers' capacity is limited, so at some point older data will be erased.

### Strobe filter configuration
Strobe filter calculates the duration of each pulse and filters the ones with duration being ```target_duration +- duration_threshold```.

You can configure the filtration of strobes via ```Motherboard.ConfigureStrobeFilter```

```python
target_duration = 16 # ms
duration_threshold = 4 # ms

ok = mb.ConfigureStrobeFilter(target_duration, duration_threshold)
```

You can read the average duration of received strobes via ```Motherboard.GetStrobeWidth()```

```python
ok, duration = mb.GetStrobeWidth() # duration is counted in ms
```

You can fine-tune the gap between receiveing the strobe and collecting the data via ```Motherboard.SetIMUStrobeOffset()``` and ```Motherboard.SetBodyStrobeOffset()``` respectively. The gap is counted in IMU timer ticks (about 800 ticks per second):

```python
ok = mb.SetIMUStrobeOffset(4)  # ticks
ok = mb.SetBodyStrobeOffset(2) # ticks
```

### Get container info
You can check the contents of the frame container via ```Motherboard.GetIMUContainerInfo()``` and ```Motherboard.GetBodyContainerInfo()``` methods respectively:

```python
ok, info = mb.GetIMUContainerInfo()
# ok, info = mb.GetBodyContainerInfo()

print("  First: " + str(info.First)) # Oldest available frame's seq number
print("  NumAv: " + str(info.NumAv)) # How many frames are available right now
print("  MaxFr: " + str(info.MaxFrames)) # The container's max capacity
```

For example, if ```info.First == 42``` and ```info.NumAv == 100```, then all the frames with seq numbers from ```42``` to ```141``` will be available.

### Clear the containers
To clear the frame containers and reset the frame counter, you should use ```Motherboard.ResetStrobeContainers()``` method:

```python
ok = mb.ResetStrobeContainers()
```

### Get frame by sequential number
You can get the frame by its sequential number via ```Motherboard.GetIMUFrame()``` and ```Motherboard.GetBodyFrame()``` respectively.

```python
ok, fr = mb.GetIMUFrame(42) # Check the availability first 

if not ok:
    print(mb.GetError())
    quit()

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

If the frame is not available right now, an error will be returned. So please check the availability of desired frame before the request.