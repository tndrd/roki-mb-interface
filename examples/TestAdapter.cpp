#include "MotherboardAdapter.hpp"
#include "RokiRcb4.hpp"
#include <cmath>

#include <chrono>
#include <thread>

int main()
{
  Roki::MotherboardAdapter mb;
  Roki::TTYConfig serviceConfig;

  serviceConfig.Port = "/dev/ttyAMA2";
  serviceConfig.Baudrate = 1250000;
  serviceConfig.ParityBit = true;
  serviceConfig.Stopbits = Roki::TTYConfig::StopbitsCount::One;
  serviceConfig.Timeout = 5;

  mb.Configure(serviceConfig);

  Roki::RokiRcb4 rcb4 {mb};
  rcb4.checkAcknowledge();

  mb.GetQueueInfo();
  mb.GetIMUInfo();
  mb.GetOrientation();
  mb.ResetIMUCounter();

  mb.GetStrobeFrame(0);
}
