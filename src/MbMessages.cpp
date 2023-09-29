#include "MbMessages.hpp"

namespace Roki {
namespace Messages {

void Byte::Serialize(uint8_t *ptr) const {
  assert(ptr);

  *ptr = Value;
}

Byte Byte::Deserialize(const uint8_t *ptr, size_t) {
  assert(ptr);

  Byte byte;
  byte.Value = *ptr;

  return byte;
}

void Empty::Serialize(uint8_t *ptr) const { assert(ptr); }

Empty Empty::Deserialize(const uint8_t *ptr, size_t) {
  assert(ptr);
  return {};
}

IMUFrame IMUFrame::Deserialize(const uint8_t *ptr, size_t) {
  assert(ptr);

  IMUFrame fr;
  // cppcheck-suppress uninitStructMember
  auto &qt = fr.Orientation;

  // cppcheck-suppress uninitStructMember
  auto &ts = fr.Timestamp;

  const float norm = 16384; // 2*14

  qt.X = float(*reinterpret_cast<const int16_t *>(ptr)) / norm;
  ptr += sizeof(int16_t);

  qt.Y = float(*reinterpret_cast<const int16_t *>(ptr)) / norm;
  ptr += sizeof(int16_t);

  qt.Z = float(*reinterpret_cast<const int16_t *>(ptr)) / norm;
  ptr += sizeof(int16_t);

  qt.W = float(*reinterpret_cast<const int16_t *>(ptr)) / norm;
  ptr += sizeof(int16_t);

  ts.TimeS = *reinterpret_cast<const uint32_t *>(ptr);
  ptr += sizeof(uint32_t);

  ts.TimeNS = *reinterpret_cast<const uint32_t *>(ptr);
  ptr += sizeof(uint32_t);

  fr.SensorID = *reinterpret_cast<const uint8_t *>(ptr);
  ptr += sizeof(uint8_t);

  return fr;
}

FrameContainerInfo FrameContainerInfo::Deserialize(const uint8_t *ptr, size_t) {
  assert(ptr);

  FrameContainerInfo info;

  info.First = *reinterpret_cast<const uint16_t *>(ptr);
  ptr += sizeof(uint16_t);

  info.NumAv = *reinterpret_cast<const uint16_t *>(ptr);
  ptr += sizeof(uint16_t);

  info.MaxFrames = *reinterpret_cast<const uint16_t *>(ptr);
  ptr += sizeof(uint16_t);

  return info;
}

void FrameNumber::Serialize(uint8_t *ptr) const {
  assert(ptr);
  *reinterpret_cast<uint16_t *>(ptr) = Seq;
}

void StrobeFilterConfig::Serialize(uint8_t *ptr) const {
  assert(ptr);
  ptr[0] = TargetDuration;
  ptr[1] = DurationThreshold;
}

Version Version::Deserialize(const uint8_t *ptr, size_t size) {
  assert(ptr);
  Version version;
  version.Major = ptr[0];
  version.Minor = ptr[1];

  return version;
}

BodyQueueInfo BodyQueueInfo::Deserialize(const uint8_t *ptr, size_t) {
  BodyQueueInfo info;

  assert(ptr);

  info.NumRequests = *reinterpret_cast<const uint16_t *>(ptr);
  ptr += sizeof(uint16_t);

  info.NumResponces = *reinterpret_cast<const uint16_t *>(ptr);
  ptr += sizeof(uint16_t);

  return info;
}

void PeriodMs::Serialize(uint8_t *ptr) const {
  assert(ptr);
  *ptr = Ms;
}

void BodyRequest::Serialize(uint8_t *ptr) const {
  assert(ptr);
  memcpy(ptr, Data, Size);
}

BodyResponce BodyResponce::Deserialize(const uint8_t *ptr, size_t size) {
  assert(ptr);
  BodyResponce responce;

  responce.Data = ptr;
  responce.Size = size;

  return responce;
}

} // namespace Msgs
} // namespace Roki