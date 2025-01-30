#pragma once

#include "roki-mb-interface/MbAliases.hpp"
#include "roki-mb-interface/MbClient.hpp"
#include "roki-mb-interface/Version.hpp"
#include "roki-mb-service/MbService.hpp"

#include <memory>
#include <mutex>

namespace MbInterface {

class Motherboard : public MbError {

private:
  MbSerial Serial;
  MbClient Client;

  std::unique_ptr<std::mutex> Mutex;

private:
  using Proc = MbService::Procedures;

private:
  bool GetVersion_NoLock(Version &result);

public:
  Motherboard();
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

  bool BodySendForward(const uint8_t *requestData, uint8_t requestSize,
                       uint8_t *responceData, uint8_t responceSize);
  bool BodySendQueue(const uint8_t *requestData, uint8_t requestSize,
                     uint8_t responceSize, uint8_t pause = 0);
  bool GetBodyQueueInfo(BodyQueueInfo &result);
  bool SetBodyQueuePeriod(uint8_t periodMs);
  bool ResetBodyQueue();

  bool GetVersion(Version &result);

  virtual ~Motherboard() = default;

  Motherboard(const Motherboard &) = delete;
  Motherboard &operator=(const Motherboard &) = delete;

  Motherboard(Motherboard &&) = default;
  Motherboard &operator=(Motherboard &&) = default;
};

} // namespace MbInterface