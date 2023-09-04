#include "Motherboard.hpp"
#include "MotherboardAliases.hpp"
#include "RokiRcb4.hpp"
#include <cmath>

#include <chrono>
#include <thread>

int main()
{
  Roki::Motherboard mb;
  Roki::TTYConfig serviceConfig;

  serviceConfig.Port = "/dev/ttyAMA2";
  serviceConfig.Baudrate = 1250000;
  serviceConfig.ParityBit = true;
  serviceConfig.Stopbits = Roki::TTYConfig::StopbitsCount::One;;
  serviceConfig.Timeout = 5;

  if (!mb.Configure(serviceConfig))
  {
    std::cout << mb.GetError() << std::endl;
    exit(1);
  }

  Roki::QueueInfo qi;

  Roki::RokiRcb4 rcb4 (mb);

  if(!rcb4.checkAcknowledge()) {
    std::cout << rcb4.GetError() << std::endl;
    exit(1);
  }


  Rcb4BaseClass::ServoData sd;
  sd.Id = 8;
  sd.Sio = 1;

  int x = 0;
  while (true)
  {
    if (x < 500)
    {
      sd.Data = 7500 + int(500 * sin(0.1 * x));
      if(!rcb4.setServoPosAsync(&sd, 1, 5)) {
        std::cout << rcb4.GetError() << std::endl;
        exit(1);
      }
    }

    x += 1;

    if (x > 500)
    {
      if(!rcb4.checkAcknowledge()) {
        std::cout << rcb4.GetError() << std::endl;
        exit(1);
      }
    }

    if (!mb.GetQueueInfo(qi))
    {
      std::cout << mb.GetError() << std::endl;
      exit(1);
    }

    std::cout << "Body info: " << std::endl;
    std::cout << " Requests: " << qi.NumRequests << std::endl;
    std::cout << " Responces: " << qi.NumResponces << std::endl;
    std::cout << std::endl;

    if (qi.NumRequests == 0)
      break;

    // std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}
