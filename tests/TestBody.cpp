#include "Motherboard.hpp"
#include "MotherboardAliases.hpp"
#include "Rcb4Adapter.hpp"
#include <cmath>

#include <chrono>
#include <thread>

int main()
{
  Roki::Motherboard mb;
  Roki::TTYConfig serviceConfig;
  Roki::TTYConfig streamConfig;

  serviceConfig.Port = "/dev/ttyAMA2";
  serviceConfig.Baudrate = 115200;
  serviceConfig.ParityBit = true;
  serviceConfig.Stopbits = serviceConfig.STOPBITS_ONE;
  serviceConfig.Timeout = 5;

  streamConfig.Port = "/dev/ttyAMA1";
  streamConfig.Baudrate = 115200;
  streamConfig.ParityBit = true;
  streamConfig.Stopbits = streamConfig.STOPBITS_ONE;
  streamConfig.Timeout = 5;

  if (!mb.Configure(serviceConfig, streamConfig))
  {
    std::cout << mb.GetError() << std::endl;
    exit(1);
  }

  Roki::Rcb4Adapter rcb4;

  rcb4.checkAcknowledge();

  bool ack = mb.BodySendSync(rcb4.GetRequestData(), rcb4.GetRequestSize(), rcb4.GetResponceData(), rcb4.GetResponceSize());

  if (!ack)
  {
    std::cout << mb.GetError() << std::endl;
    exit(1);
  }

  Rcb4BaseClass::ServoData sd;
  sd.Id = 8;
  sd.Sio = 1;

  int x = 0;
  while (x < 500)
  {
    sd.Data = 7500 + int(500 * sin(0.1 * x));
    rcb4.setServoPos(&sd, 1, 5);
    bool ret = mb.BodySendAsync(rcb4.GetRequestData(), rcb4.GetRequestSize(), rcb4.GetResponceSize());
    x += 1;

    if (!ret)
      std::cout << mb.GetError() << std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  rcb4.checkAcknowledge();
  ack = mb.BodySendSync(rcb4.GetRequestData(), rcb4.GetRequestSize(), rcb4.GetResponceData(), rcb4.GetResponceSize());

  if (!ack)
  {
    std::cout << mb.GetError() << std::endl;
    exit(1);
  }

  return 0;
}
