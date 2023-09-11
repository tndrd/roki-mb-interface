#include "Motherboard.hpp"

namespace Roki {

bool Motherboard::MakeError(const std::string &msg) {
  HasError = true;
  Error = msg;
  return false;
}

bool Motherboard::ServiceError() {
  return MakeError("Service: " + Service.GetError());
}

bool Motherboard::IMUError() { return MakeError("IMU: " + IMU.GetError()); }

bool Motherboard::BodyError() { return MakeError("Body: " + Body.GetError()); }

bool Motherboard::Configure(const SerialInterface::TTYConfig &ServiceConfig) {
  if (!Service.Configure(ServiceConfig))
    return ServiceError();

  if (!Service.CheckVersion())
    return ServiceError();

  return true;
}

bool Motherboard::GetOrientation(uint16_t seq, IMURPC::IMUFrame &result) {
  IMURPC::IMUFrameRequest request{seq};
  IMURPC::IMUFrame responce;

  if (!IMU.PerformRPC(Service, request, responce))
    return IMUError();

  result = responce;
  return true;
}

bool Motherboard::GetOrientation(IMURPC::IMUFrame &result) {
  IMURPC::IMULatestRequest request{};
  IMURPC::IMUFrame responce;

  if (!IMU.PerformRPC(Service, request, responce))
    return IMUError();

  result = responce;
  return true;
}

bool Motherboard::GetIMUInfo(IMURPC::IMUInfo &result) {
  IMURPC::IMUInfoRequest request{};
  IMURPC::IMUInfo responce;

  if (!IMU.PerformRPC(Service, request, responce))
    return IMUError();

  result = responce;
  return true;
}

bool Motherboard::ResetIMUCounter() {
  IMURPC::IMUResetRequest request{};
  IMURPC::Empty responce;

  if (!IMU.PerformRPC(Service, request, responce))
    return IMUError();
  return true;
}

bool Motherboard::SetStrobeOffset(uint8_t offset) {
  IMURPC::SetStrobeOffsetRequest request{offset};
  IMURPC::Empty responce;

  if (!IMU.PerformRPC(Service, request, responce))
    return IMUError();
  return true;
}

bool Motherboard::GetStrobeWidth(uint8_t& width) {
  IMURPC::StrobeWidthRequest request {};
  IMURPC::Byte responce;

  if (!IMU.PerformRPC(Service, request, responce))
    return IMUError();

  width = responce.Value;
  return true;
}

bool Motherboard::BodySendSync(const uint8_t *requestData, uint8_t requestSize,
                               uint8_t *responceData, uint8_t responceSize) {
  BodyRPC::Request request;
  request.Data = requestData;
  request.RequestSize = requestSize;
  request.ResponceSize = responceSize;
  request.Mode = BodyRPC::MessageMode::Sync;

  if (!Body.Send(Service, request))
    return BodyError();

  BodyRPC::Responce responce;
  if (!Body.Recieve(Service, responce, responceSize))
    return BodyError();

  memcpy(responceData, responce.Data, responceSize);
  return true;
}

bool Motherboard::BodySendAsync(const uint8_t *requestData, uint8_t requestSize,
                                uint8_t responceSize) {
  BodyRPC::Request request;
  request.Data = requestData;
  request.RequestSize = requestSize;
  request.ResponceSize = responceSize;
  request.Mode = BodyRPC::MessageMode::Async;

  if (!Body.Send(Service, request))
    return BodyError();

  return true;
}

bool Motherboard::GetQueueInfo(BodyRPC::Info& result) {
  if (!Body.GetInfo(Service, result))
    return BodyError();
  
  return true;
}

bool Motherboard::IsOk() const { return HasError; }
std::string Motherboard::GetError() const { return Error; }

} // namespace Roki