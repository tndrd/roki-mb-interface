#include "MotherboardAdapter.hpp"
#include "Rcb4Adapter.hpp"
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
  serviceConfig.Stopbits = serviceConfig.STOPBITS_ONE;
  serviceConfig.Timeout = 5;

  mb.Configure(serviceConfig);

  Roki::Rcb4Adapter rcb4;
  rcb4.checkAcknowledge();

  std::vector<uint8_t> request;
  request.resize(rcb4.GetRequestSize());
  std::copy(rcb4.GetRequestData(), rcb4.GetRequestData() + rcb4.GetRequestSize(), request.begin());

  auto responce =  mb.BodySendSync(request, rcb4.GetResponceSize());
  mb.BodySendAsync(request, rcb4.GetResponceSize());
  
  mb.GetQueueInfo();
  mb.GetIMUInfo();
  mb.GetCurrentOrientation();
  mb.ResetIMUCounter();

  mb.GetOrientationBySeq(0);
}
