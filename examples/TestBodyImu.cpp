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
  serviceConfig.Stopbits = Roki::TTYConfig::StopbitsCount::One;
  ;
  serviceConfig.Timeout = 20;

  if (!mb.Configure(serviceConfig))
  {
    std::cout << mb.GetError() << std::endl;
    exit(1);
  }

  Roki::RokiRcb4 rcb4(mb);

  if (!rcb4.checkAcknowledge())
  {
    std::cout << rcb4.GetError() << std::endl;
    exit(1);
  }

  while (true)
  {
    uint8_t rxData[8] = {};
    if (!rcb4.moveRamToComCmdSynchronize(0x0060, 8, rxData))
    {
      std::cout << rcb4.GetError() << std::endl;
      exit(1);
    }

    for (int i = 0; i < 8; ++i)
      std::cout << +rxData[i] << " ";
    std::cout << std::endl;
  }

  return 0;
}
