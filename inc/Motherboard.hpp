#pragma once
#include "MbAliases.hpp"
#include "MbClient.hpp"
#include "MbService.hpp"
#include "Version.hpp"

namespace Roki {

class Motherboard {
private:
  bool HasError = false;
  std::string Error;

  MbSerial Serial;
  MbClient Client;
private:
  using Proc = MbService::Procedures;

private:
  bool MakeError(const std::string &msg);
  bool MakeFooError(const std::string& fooName, const std::string& msg);
  
public:
  Motherboard() = default;
  bool Configure(const TTYConfig &serviceConfig);

  bool GetIMUFrame(uint16_t seq, IMUFrame &result);
  bool GetBodyFrame(uint16_t seq, BodyResponce &result);

  bool GetIMUContainerInfo(FrameContainerInfo &result);
  bool GetBodyContainerInfo(FrameContainerInfo &result);

  bool ResetStrobeContainers();

  bool SetIMUStrobeOffset(uint8_t offset);
  bool SetBodyStrobeOffset(uint8_t offset);

  bool GetIMULatest(IMUFrame &result);

  bool GetStrobeWidth(uint8_t &result);
  bool ConfigureStrobeFilter(uint8_t targetDuration, uint8_t durationThreshold);

  bool BodySendForward(const uint8_t* requestData, uint8_t requestSize, uint8_t* responceData, uint8_t responceSize);
  bool BodySendQueue(const uint8_t* requestData, uint8_t requestSize, uint8_t responceSize);
  bool GetBodyQueueInfo(BodyQueueInfo& result);
  bool SetBodyQueuePeriod(uint8_t periodMs);

  bool GetVersion(Version& result);

  bool IsOk() const;
  std::string GetError() const;
};

} // namespace Roki