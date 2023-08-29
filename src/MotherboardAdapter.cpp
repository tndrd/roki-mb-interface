#include "MotherboardAdapter.hpp"

namespace Roki {
MotherboardException::MotherboardException(const std::string &source,
                                           const std::string &msg)
    : std::runtime_error("Motherboard::" + source + ": " + msg) {}

void MotherboardAdapter::Configure(
    const SerialInterface::TTYConfig &serviceConfig) {
  static const char *FooName = "GetOrientationBySeq";

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
  static const char *FooName = "GetOrientationBySeq";

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

std::vector<uint8_t>
MotherboardAdapter::BodySendSync(const std::vector<uint8_t> &txData,
                                 int responceSize) {
  static const char *FooName = "BodySendSync";

  if (responceSize < 0 || responceSize > std::numeric_limits<uint16_t>::max())
    throw MotherboardException(FooName, "Wrong responce size: " +
                                            std::to_string(responceSize));

  if (txData.empty())
    throw MotherboardException(FooName, "Wrong request size" +
                                            std::to_string(txData.size()));

  std::vector<uint8_t> rxData(responceSize, 0);

  if (!Motherboard::BodySendSync(txData.data(), txData.size(), rxData.data(),
                                 responceSize))
    throw ForwardException(FooName);

  return rxData;
}

void MotherboardAdapter::BodySendAsync(const std::vector<uint8_t> &txData,
                                       int responceSize) {
  static const char *FooName = "BodySendAsync";

  if (responceSize < 0 || responceSize > std::numeric_limits<uint16_t>::max())
    throw MotherboardException(FooName, "Wrong responce size: " +
                                            std::to_string(responceSize));

  if (txData.empty())
    throw MotherboardException(FooName, "Wrong request size" +
                                            std::to_string(txData.size()));

  if (!Motherboard::BodySendAsync(txData.data(), txData.size(), responceSize))
    throw ForwardException(FooName);
}

QueueInfo MotherboardAdapter::GetQueueInfo() {
  static const char *FooName = "GetQueueInfo";

  QueueInfo result;

  if (!Motherboard::GetQueueInfo(result))
    throw ForwardException(FooName);

  return result;
}
} // namespace Roki