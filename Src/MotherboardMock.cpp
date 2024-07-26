#include "roki-mb-interface/Motherboard.hpp"

namespace MbInterface {

#define MOCK                                                                   \
  { return true; }

#define MOCK_RESULT                                                            \
  {                                                                            \
    result = {};                                                               \
    return true;                                                               \
  }

bool Motherboard::Configure(const TTYConfig &config) MOCK;

bool Motherboard::GetIMUFrame(uint16_t seq, IMUFrame &result) MOCK_RESULT;

bool Motherboard::GetBodyFrame(uint16_t seq, BodyResponce &result) MOCK_RESULT;

bool Motherboard::GetIMUContainerInfo(FrameContainerInfo &result) MOCK_RESULT;

bool Motherboard::GetBodyContainerInfo(FrameContainerInfo &result) MOCK_RESULT;

bool Motherboard::ResetStrobeContainers() MOCK;

bool Motherboard::SetIMUStrobeOffset(uint8_t offset) MOCK;

bool Motherboard::SetBodyStrobeOffset(uint8_t offset) MOCK;

bool Motherboard::GetIMULatest(IMUFrame &result) MOCK_RESULT;

bool Motherboard::GetStrobeWidth(uint8_t &result) MOCK_RESULT;

bool Motherboard::ConfigureStrobeFilter(uint8_t targetDuration,
                                        uint8_t durationThreshold) MOCK;

bool Motherboard::BodySendForward(const uint8_t *requestData,
                                  uint8_t requestSize, uint8_t *responceData,
                                  uint8_t responceSize) MOCK;

bool Motherboard::BodySendQueue(const uint8_t *requestData, uint8_t requestSize,
                                uint8_t responceSize, uint8_t pause) MOCK;

bool Motherboard::GetBodyQueueInfo(BodyQueueInfo &result) MOCK_RESULT;

bool Motherboard::SetBodyQueuePeriod(uint8_t periodMs) MOCK;

bool Motherboard::GetVersion_NoLock(Version &result) MOCK_RESULT;

bool Motherboard::GetVersion(Version &result) MOCK_RESULT;

bool Motherboard::ResetBodyQueue() MOCK;

bool Motherboard::IsOk() const MOCK;

std::string Motherboard::GetError() const { return "OK"; }

bool Motherboard::MakeError(const std::string &msg) { return false; }

bool Motherboard::MakeFooError(const std::string &fooName,
                               const std::string &msg) {
  return false;
}

Motherboard::Motherboard(): Mutex{std::make_unique<std::mutex>()} {};

} // namespace MbInterface