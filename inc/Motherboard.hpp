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
  SerialInterface Stream;

  IMURPC IMU;
  BodyRPC Body;

private:
  bool MakeError(const std::string &msg);
  bool ServiceError();
  bool StreamError();
  bool IMUError();
  bool BodyError();

public:
  Motherboard() = default;

  bool Configure(const SerialInterface::TTYConfig &ServiceConfig,
                 const SerialInterface::TTYConfig &StreamConfig);

  bool GetOrientation(uint16_t seq, IMURPC::IMUFrame &result);
  bool GetOrientation(IMURPC::IMUFrame &result);
  bool GetIMUInfo(IMURPC::IMUInfo &result);
  bool ResetIMUCounter();

  bool BodySendSync(const uint8_t *requestData, uint8_t requestSize,
                    uint8_t *responceData, uint8_t responceSize);
  bool BodySendAsync(const uint8_t *requestData, uint8_t requestSize,
                     uint8_t responceSize);

  bool IsOk() const;
  std::string GetError() const;
};

} // namespace Roki