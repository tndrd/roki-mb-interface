#!/bin/bash
set -e

echo "Downloading dependencies..."

sudo apt install cmake git
sudo apt install python3 python3-dev python3-distutils

echo "Updating submodules..."

git submodule update --init --recursive

echo "Building..."

mkdir -p build
cd build
cmake ..
cmake --build .

echo "Installing..."

cmake --install .

echo "Installation successful"