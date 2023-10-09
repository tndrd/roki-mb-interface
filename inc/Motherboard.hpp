#pragma once
#include "BodyRPC.hpp"
#include "IMURPC.hpp"
#include "SerialInterface.hpp"

namespace Roki {

class Motherboard {
private:
  bool HasError = false;
  std::string Error;

  SerialInterface Service;

  IMURPC IMU;
  BodyRPC Body;

private:
  bool MakeError(const std::string &msg);
  bool ServiceError();
  bool IMUError();
  bool BodyError();

public:
  Motherboard() = default;

  bool Configure(const SerialInterface::TTYConfig &serviceConfig);
  bool GetOrientation(uint16_t seq, IMURPC::IMUFrame &result);
  bool GetOrientation(IMURPC::IMUFrame &result);
  bool GetIMUInfo(IMURPC::IMUInfo &result);
  bool ResetIMUCounter();
  bool ResetQueue();
  bool SetStrobeOffset(uint8_t offset);
  bool GetStrobeWidth(uint8_t& width);
  bool ConfigureStrobeFilter(uint8_t targetDuration, uint8_t durationThreshold);

  bool BodySendSync(const uint8_t *requestData, uint8_t requestSize,
                    uint8_t *responceData, uint8_t responceSize);
  bool BodySendAsync(const uint8_t *requestData, uint8_t requestSize,
                     uint8_t responceSize);

  bool GetQueueInfo(BodyRPC::Info& result);
  bool SetQueuePeriod(uint8_t periodMs);
  
  bool IsOk() const;
  std::string GetError() const;
};

} // namespace Roki