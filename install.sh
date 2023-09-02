echo "Downloading dependencies..."

sudo apt install cmake
sudo apt install python3.8 python3-dev python3.8-distutils

echo "Updating submnodules..."

git submodule update

echo "Building..."

cd roki-mb-interface
mkdir build
cd build
cmake ..
cmake --build .

echo "Installing..."

cmake --install .

echo "Installation successful"