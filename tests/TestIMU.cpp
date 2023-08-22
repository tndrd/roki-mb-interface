#include "MBInterface.hpp"
#include "IMUInterface.hpp"

int main() {
  Roki::MBInterface mbi;
  Roki::MBInterface::TTYConfig tty;

  tty.Port = "/dev/ttyAMA2";
  tty.Baudrate = 1250000;
  tty.ParityBit = false;
  tty.Stopbits = tty.STOPBITS_ONE;
  tty.Timeout = 2;

  if(!mbi.Configure(tty)) {
    std::cout << mbi.GetError() << std::endl;
    exit(1);
  }

  Roki::IMUInterface imu;
  Roki::IMUInterface::IMUFrame frame;

  if(!imu.GetFrame(mbi, 1, frame)) {
    std::cout << imu.GetError() << std::endl;
    exit(1);
  }

  std::cout << "Quaternion: " << std::endl;
  std::cout << "  x:   " << frame.Orientation.X << std::endl;
  std::cout << "  y:   " << frame.Orientation.Y << std::endl;
  std::cout << "  z:   " << frame.Orientation.Z << std::endl;
  std::cout << "  w:   " << frame.Orientation.W << std::endl;
  std::cout << "  Acc: " << frame.Orientation.Accuracy << std::endl;

  std::cout << "Timestamp: " << std::endl;
  std::cout << "  s:   " << frame.Timestamp.TimeS << std::endl;
  std::cout << "  ns:  " << frame.Timestamp.TimeNS << std::endl;
  
  std::cout << "Sensor ID: " << frame.SensorID << std::endl;

  return 0;
}