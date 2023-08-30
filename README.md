# Humaniod robot "Roki" - Motherboard MCU communication library 
 Library provides communication interface between "Roki"s RaspberryPi and MCU unit embedded in motherboard PCB.

 # Build process
```bash
git clone https://github.com/tndrd/roki-mb-interface.git --recursive
cd roki-mb-interface
mkdir build
cd build
cmake ..
cmake --build .
```
# Troubleshooting
If you got any errors while building pybind11, try following (Ubuntu):
```bash
sudo apt-get install python3-dev python3.8-distutils
```