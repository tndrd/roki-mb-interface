#include "Motherboard.hpp"
#include "MotherboardAliases.hpp"

#include <chrono>
#include <thread>

int main()
{
  Roki::Motherboard mb;
  Roki::TTYConfig serviceConfig;

  serviceConfig.Port = "/dev/ttyAMA2";
  serviceConfig.Baudrate = 1250000;
  serviceConfig.ParityBit = true;
  serviceConfig.Stopbits = Roki::TTYConfig::StopbitsCount::One;
  serviceConfig.Timeout = 2;

  if (!mb.Configure(serviceConfig))
  {
    std::cout << mb.GetError() << std::endl;
    exit(1);
  }

  Roki::IMUInfo info;
  Roki::IMUFrame frame;

  uint8_t fr[5]{0, 1, 2, 3, 4};
  int i = -1;
  int j = 0;

  while (false)
  {
    if (j++ == 5)
      if (!mb.ResetIMUCounter())
      {
        std::cout << mb.GetError() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        continue;
      }

    if (!mb.GetIMUInfo(info))
    {
      std::cout << mb.GetError() << std::endl;
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
    if (!mb.GetOrientation(frame))
    {
      std::cout << mb.GetError() << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
      continue;
    }

    std::cout << "Quaternion: " << std::endl;
    std::cout << "  x:  " << frame.Orientation.X << std::endl;
    std::cout << "  y:  " << frame.Orientation.Y << std::endl;
    std::cout << "  z:  " << frame.Orientation.Z << std::endl;
    std::cout << "  w:  " << frame.Orientation.W << std::endl;
    // std::cout << "  Acc: " << frame.Orientation.Accuracy << std::endl;

    std::cout << "Timestamp: " << std::endl;
    std::cout << "  s:  " << frame.Timestamp.TimeS << std::endl;
    std::cout << "  ns: " << frame.Timestamp.TimeNS << std::endl;

    std::cout << "Sensor ID: " << +frame.SensorID << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }
  return 0;
}
