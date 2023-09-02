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

# Usage (Python)
The module is delivered as shared library (*.so file). To include it in your python code, you need to specify where to find