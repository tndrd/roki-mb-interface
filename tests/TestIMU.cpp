#include "MBInterface.hpp"
#include "IMUInterface.hpp"

#include <chrono>
#include <thread>

int main()
{
  Roki::MBInterface mbi;
  Roki::MBInterface::TTYConfig tty;

  tty.Port = "/dev/ttyAMA2";
  tty.Baudrate = 9600;
  tty.ParityBit = true;
  tty.Stopbits = tty.STOPBITS_ONE;
  tty.Timeout = 2;

  if (!mbi.Configure(tty))
  {
    std::cout << mbi.GetError() << std::endl;
    exit(1);
  }

  Roki::IMUInterface imu;
  Roki::IMUInterface::IMUFrame frame;
  Roki::IMUInterface::IMUInfo info;

  uint8_t fr[5]{0, 1, 2, 3, 4};
  int i = -1;
  int j = 0;

  while (false)
  {
    if (j++ == 5)
      if (!imu.ResetCounter(mbi))
      {
        std::cout << imu.GetError() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        continue;
      }

    if (!imu.GetInfo(mbi, info))
    {
      std::cout << imu.GetError() << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
      continue;
    }

    std::cout << "Imu info: " << std::endl;

    std::cout << "  First:  " << info.First << std::endl;
    std::cout << "  NumAv:  " << info.NumAv << std::endl;

    std::cout << std::endl;
  }

  while (true)
  {
    i = (i + 1) % 500;
    if (!imu.GetFrame(mbi, i, frame))
    {
      std::cout << imu.GetError() << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
      continue;
    }

    std::cout << "Quaternion: " << +i << std::endl;
    std::cout << "  x:  " << frame.Orientation.X << std::endl;
    std::cout << "  y:  " << frame.Orientation.Y << std::endl;
    std::cout << "  z:  " << frame.Orientation.Z << std::endl;
    std::cout << "  w:  " << frame.Orientation.W << std::endl;
    // std::cout << "  Acc: " << frame.Orientation.Accuracy << std::endl;

    std::cout << "Timestamp: " << std::endl;
    std::cout << "  s:  " << frame.Timestamp.TimeS << std::endl;
    std::cout << "  ns: " << frame.Timestamp.TimeNS << std::endl;

    std::cout << "Sensor ID: " << +frame.SensorID << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
}
