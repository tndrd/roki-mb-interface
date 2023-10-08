#include "MotherboardAdapter.hpp"

namespace Roki {

using MA = MotherboardAdapter;
#define ERR_RET                                                                \
  {                                                                            \
    false, {}                                                                  \
  }

bool MA::MakeError(const std::string &msg) {
  HasError = true;
  Error = msg;
  return false;
}

template <typename T> bool MA::CheckIntBoundaries(int value) {
  static const char *msg = "Inappropriate argument value";

  if (value < std::numeric_limits<T>::min())
    return MakeError(msg);

  if (value > std::numeric_limits<T>::max())
    return MakeError(msg);

  return true;
}

MA::Ret<IMUFrame> MA::GetIMUFrame(int seq) {
  if (!CheckIntBoundaries<uint16_t>(seq))
    return ERR_RET;

  IMUFrame frame;
  bool result = Motherboard::GetIMUFrame(seq, frame);

  return {result, frame};
}

MA::Ret<std::vector<uint8_t>> MA::GetBodyFrame(int seq) {
  if (!CheckIntBoundaries<uint16_t>(seq))
    return ERR_RET;

  BodyResponce bodyResponce;
  bool result = Motherboard::GetBodyFrame(seq, bodyResponce);

  if (!result)
    return ERR_RET;

  std::vector<uint8_t> data;
  data.resize(bodyResponce.Size);

  memcpy(data.data(), bodyResponce.Data, bodyResponce.Size);

  return {result, data};
}

MA::Ret<FrameContainerInfo> MA::GetIMUContainerInfo() {
  FrameContainerInfo info;
  bool result = Motherboard::GetIMUContainerInfo(info);

  return {result, info};
}

MA::Ret<FrameContainerInfo> MA::GetBodyContainerInfo() {
  FrameContainerInfo info;
  bool result = Motherboard::GetBodyContainerInfo(info);

  return {result, info};
}

bool MA::ResetStrobeContainers() {
  return Motherboard::ResetStrobeContainers();
}

bool MA::SetIMUStrobeOffset(int offset) {
  if (!CheckIntBoundaries<uint8_t>(offset))
    return false;

  return Motherboard::SetIMUStrobeOffset(offset);
}
bool MA::SetBodyStrobeOffset(int offset) {
  if (!CheckIntBoundaries<uint8_t>(offset))
    return false;

  return Motherboard::SetBodyStrobeOffset(offset);
}

MA::Ret<IMUFrame> MA::GetIMULatest() {
  IMUFrame frame;
  bool result = Motherboard::GetIMULatest(frame);
  return {result, frame};
}

MA::Ret<int> MA::GetStrobeWidth() {
  uint8_t width;
  bool result = Motherboard::GetStrobeWidth(width);
  return {result, width};
}

bool MA::ConfigureStrobeFilter(int targetDuration, int durationThreshold) {
  if (!CheckIntBoundaries<uint8_t>(targetDuration))
    return false;
  if (!CheckIntBoundaries<uint8_t>(durationThreshold))
    return false;

  return Motherboard::ConfigureStrobeFilter(targetDuration, durationThreshold);
}

MA::Ret<BodyQueueInfo> MA::GetBodyQueueInfo() {
  BodyQueueInfo info;
  bool result = Motherboard::GetBodyQueueInfo(info);
  return {result, info};
}

bool MA::SetBodyQueuePeriod(int periodMs) {
  if (!CheckIntBoundaries<uint8_t>(periodMs))
    return false;
  
  return Motherboard::SetBodyQueuePeriod(periodMs);
}

MA::Ret<Version> MA::GetVersion() {
  Version version;
  bool result = Motherboard::GetVersion(version);
  return {result, version};
}

} // namespace Roki