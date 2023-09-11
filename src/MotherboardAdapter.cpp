#include "MotherboardAdapter.hpp"

namespace Roki {
MotherboardException::MotherboardException(const std::string &source,
                                           const std::string &msg)
    : std::runtime_error("Motherboard::" + source + ": " + msg) {}

void MotherboardAdapter::Configure(
    const SerialInterface::TTYConfig &serviceConfig) {
  static const char *FooName = "Configure";

  if (!Motherboard::Configure(serviceConfig))
    throw ForwardException(FooName);
}

MotherboardException MotherboardAdapter::ForwardException(const std::string &source) const {
  return MotherboardException(source, Motherboard::GetError());
}

IMUFrame MotherboardAdapter::GetOrientationBySeq(int seq) {
  static const char *FooName = "GetOrientationBySeq";

  if (seq < 0 || seq > std::numeric_limits<uint16_t>::max())
    throw MotherboardException(FooName, "Wrong argument value");

  IMUFrame result;

  if (!Motherboard::GetOrientation(static_cast<uint16_t>(seq), result))
    throw ForwardException(FooName);

  return result;
}

IMUFrame MotherboardAdapter::GetCurrentOrientation() {
  static const char *FooName = "GetCurrentOrientation";

  IMUFrame result;

  if (!Motherboard::GetOrientation(result))
    throw ForwardException(FooName);

  return result;
}

IMUInfo MotherboardAdapter::GetIMUInfo() {
  static const char *FooName = "GetIMUInfo";

  IMUInfo result;

  if (!Motherboard::GetIMUInfo(result))
    throw ForwardException(FooName);

  return result;
}

void MotherboardAdapter::ResetIMUCounter() {
  static const char *FooName = "ResetIMUCounter";

  if (!Motherboard::ResetIMUCounter())
    throw ForwardException(FooName);
}

void MotherboardAdapter::SetStrobeOffset(int offset) {
  static const char *FooName = "SetStrobeOffset";

  if (offset < 0 || offset > std::numeric_limits<uint8_t>::max())
    throw MotherboardException(FooName, "Wrong argument value");

  if (!Motherboard::SetStrobeOffset(offset))
    throw ForwardException(FooName);
}

int MotherboardAdapter::GetStrobeWidth() {
  static const char *FooName = "GetStrobeWidth";

  uint8_t width;

  if (!Motherboard::GetStrobeWidth(width))
    throw ForwardException(FooName);

  return width;
}

QueueInfo MotherboardAdapter::GetQueueInfo() {
  static const char *FooName = "GetQueueInfo";

  QueueInfo result;

  if (!Motherboard::GetQueueInfo(result))
    throw ForwardException(FooName);

  return result;
}
} // namespace Roki