#include "MbMessages.hpp"

namespace Roki {
namespace Messages {

void Byte::Serialize(uint8_t *ptr) const {
  assert(ptr);

  *ptr = Value;
}

Byte Byte::Deserialize(const uint8_t *ptr) {
  assert(ptr);

  Byte byte;
  byte.Value = *ptr;

  return byte;
}

size_t Byte::GetPackedSize() const { return 1; }

void Empty::Serialize(uint8_t *ptr) const { assert(ptr); }

Empty Empty::Deserialize(const uint8_t *ptr) {
  assert(ptr);
  return {};
}

size_t Empty::GetPackedSize() const { return 1; }

void IMUFrameMsg::Serialize(uint8_t *ptr) const {
  assert(ptr);
  auto &qt = Orientation;
  auto &ts = Timestamp;

  *reinterpret_cast<int16_t *>(ptr) = qt.X;
  ptr += sizeof(int16_t);

  *reinterpret_cast<int16_t *>(ptr) = qt.Y;
  ptr += sizeof(int16_t);

  *reinterpret_cast<int16_t *>(ptr) = qt.Z;
  ptr += sizeof(int16_t);

  *reinterpret_cast<int16_t *>(ptr) = qt.W;
  ptr += sizeof(int16_t);

  *reinterpret_cast<uint32_t *>(ptr) = ts.TimeS;
  ptr += sizeof(uint32_t);

  *reinterpret_cast<uint32_t *>(ptr) = ts.TimeNS;
  ptr += sizeof(uint32_t);

  *reinterpret_cast<uint8_t *>(ptr) = SensorID;
  ptr += sizeof(uint8_t);
}

IMUFrameMsg IMUFrameMsg::Deserialize(const uint8_t *ptr) {
  assert(ptr);

  IMUFrameMsg fr;
  // cppcheck-suppress uninitStructMember
  auto &qt = fr.Orientation;

  // cppcheck-suppress uninitStructMember
  auto &ts = fr.Timestamp;

  qt.X = *reinterpret_cast<const int16_t *>(ptr);
  ptr += sizeof(int16_t);

  qt.Y = *reinterpret_cast<const int16_t *>(ptr);
  ptr += sizeof(int16_t);

  qt.Z = *reinterpret_cast<const int16_t *>(ptr);
  ptr += sizeof(int16_t);

  qt.W = *reinterpret_cast<const int16_t *>(ptr);
  ptr += sizeof(int16_t);

  ts.TimeS = *reinterpret_cast<const uint32_t *>(ptr);
  ptr += sizeof(uint32_t);

  ts.TimeNS = *reinterpret_cast<const uint32_t *>(ptr);
  ptr += sizeof(uint32_t);

  fr.SensorID = *reinterpret_cast<const uint8_t *>(ptr);
  ptr += sizeof(uint8_t);

  return fr;
}

size_t IMUFrameMsg::GetPackedSize() const {
  return 4 * sizeof(int16_t) + 2 * sizeof(uint32_t) + sizeof(uint8_t);
}

void FrameContainerInfo::Serialize(uint8_t *ptr) const {
  assert(ptr);

  *reinterpret_cast<uint16_t *>(ptr) = First;
  ptr += sizeof(uint16_t);

  *reinterpret_cast<uint16_t *>(ptr) = NumAv;
  ptr += sizeof(uint16_t);

  *reinterpret_cast<uint16_t *>(ptr) = MaxFrames;
  ptr += sizeof(uint16_t);
}

FrameContainerInfo FrameContainerInfo::Deserialize(const uint8_t *ptr) {
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

size_t FrameContainerInfo::GetPackedSize() const {
  return 3 * sizeof(uint16_t);
}

void FrameNumber::Serialize(uint8_t *ptr) const {
  assert(ptr);
  *reinterpret_cast<uint16_t *>(ptr) = Seq;
}

FrameNumber FrameNumber::Deserialize(const uint8_t *ptr) {
  assert(ptr);
  FrameNumber seq;
  seq.Seq = *reinterpret_cast<const uint16_t *>(ptr);

  return seq;
}

size_t FrameNumber::GetPackedSize() const { return sizeof(uint16_t); }

void StrobeFilterConfig::Serialize(uint8_t *ptr) const {
  assert(ptr);
  ptr[0] = TargetDuration;
  ptr[1] = DurationThreshold;
}

StrobeFilterConfig StrobeFilterConfig::Deserialize(const uint8_t *ptr) {
  assert(ptr);

  StrobeFilterConfig conf;
  conf.TargetDuration = ptr[0];
  conf.DurationThreshold = ptr[1];

  return conf;
}

size_t StrobeFilterConfig::GetPackedSize() const { return 2 * sizeof(uint8_t); }

void Version::Serialize(uint8_t *ptr) const {
  assert(ptr);

  ptr[0] = Major;
  ptr[1] = Minor;
}

Version Version::Deserialize(const uint8_t *ptr) {
  assert(ptr);
  Version version;
  version.Major = ptr[0];
  version.Minor = ptr[1];

  return version;
}

size_t Version::GetPackedSize() const { return 2 * sizeof(uint8_t); }

void BodyQueueInfo::Serialize(uint8_t *ptr) const {
  assert(ptr);

  *reinterpret_cast<uint16_t *>(ptr) = NumRequests;
  ptr += sizeof(uint16_t);

  *reinterpret_cast<uint16_t *>(ptr) = NumResponces;
  ptr += sizeof(uint16_t);
}

BodyQueueInfo BodyQueueInfo::Deserialize(const uint8_t *ptr) {
  BodyQueueInfo info;

  assert(ptr);

  info.NumRequests = *reinterpret_cast<const uint16_t *>(ptr);
  ptr += sizeof(uint16_t);

  info.NumResponces = *reinterpret_cast<const uint16_t *>(ptr);
  ptr += sizeof(uint16_t);

  return info;
}

size_t BodyQueueInfo::GetPackedSize() const { return 2 * sizeof(uint16_t); }

void PeriodMs::Serialize(uint8_t *ptr) const {
  assert(ptr);
  *ptr = Ms;
}

PeriodMs PeriodMs::Deserialize(const uint8_t *ptr) {
  assert(ptr);
  PeriodMs period;

  period.Ms = *ptr;

  return period;
}

size_t PeriodMs::GetPackedSize() const { return sizeof(uint8_t); }

void BodyRequest::Serialize(uint8_t *ptr) const {
  assert(ptr);
  ptr[0] = RequestSize;
  ptr[1] = ResponceSize;

  memcpy(ptr + 2, Data, RequestSize);
}

BodyRequest BodyRequest::Deserialize(const uint8_t *ptr) {
  assert(ptr);
  BodyRequest request;

  request.RequestSize = ptr[0];
  request.ResponceSize = ptr[1];

  request.Data = ptr + 2;
}

size_t BodyRequest::GetPackedSize() const {
  return (2 + RequestSize) * sizeof(uint8_t);
}

void BodyResponce::Serialize(uint8_t *ptr) const {
  assert(ptr);
  ptr[0] = ResponceSize;
  memcpy(ptr + 1, Data, ResponceSize);
}

BodyResponce BodyResponce::Deserialize(const uint8_t *ptr) {
  assert(ptr);
  BodyResponce responce;

  responce.ResponceSize = ptr[0];
  responce.Data = ptr + 1;

  return responce;
}

size_t BodyResponce::GetPackedSize() const {
  return (1 + ResponceSize) * sizeof(uint8_t);
}

} // namespace Messages
} // namespace Roki