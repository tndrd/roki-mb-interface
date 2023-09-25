#include "MotherboardAdapter.hpp"

namespace Roki
{
  MotherboardException::MotherboardException(const std::string &source,
                                             const std::string &msg)
      : std::runtime_error("Motherboard::" + source + ": " + msg) {}

  void MotherboardAdapter::Configure(
      const SerialInterface::TTYConfig &serviceConfig)
  {
    static const char *fooName = "Configure";

    if (!Motherboard::Configure(serviceConfig))
      throw ForwardException(fooName);
  }

  MotherboardException MotherboardAdapter::ForwardException(const std::string &source) const
  {
    return MotherboardException(source, Motherboard::GetError());
  }

  template<typename T>
  void MotherboardAdapter::CheckIntBoundaries(int value, const char* fooName) const {
    static const char* msg = "Inappropriate argument value";

    if (value < std::numeric_limits<T>::min())
      throw MotherboardException(fooName, msg);

    if (value > std::numeric_limits<T>::max())
      throw MotherboardException(fooName, msg);
  }

  StrobeFrame MotherboardAdapter::GetStrobeFrame(int seq)
  {
    static const char *fooName = "GetStrobeFrame";

    CheckIntBoundaries<uint16_t>(seq, fooName);

    StrobeFrame result;

    if (!Motherboard::GetStrobeFrame(static_cast<uint16_t>(seq), result))
      throw ForwardException(fooName);

    return result;
  }

  IMUFrame MotherboardAdapter::GetOrientation()
  {
    static const char *fooName = "GetOrientation";

    IMUFrame result;

    if (!Motherboard::GetOrientation(result))
      throw ForwardException(fooName);

    return result;
  }

  IMUInfo MotherboardAdapter::GetIMUInfo()
  {
    static const char *fooName = "GetIMUInfo";

    IMUInfo result;

    if (!Motherboard::GetIMUInfo(result))
      throw ForwardException(fooName);

    return result;
  }

  void MotherboardAdapter::ResetIMUCounter()
  {
    static const char *fooName = "ResetIMUCounter";

    if (!Motherboard::ResetIMUCounter())
      throw ForwardException(fooName);
  }

  void MotherboardAdapter::SetStrobeOffset(int offset)
  {
    static const char *fooName = "SetStrobeOffset";

    CheckIntBoundaries<uint8_t>(offset, fooName);

    if (!Motherboard::SetStrobeOffset(offset))
      throw ForwardException(fooName);
  }

  int MotherboardAdapter::GetStrobeWidth()
  {
    static const char *fooName = "GetStrobeWidth";

    uint8_t width;

    if (!Motherboard::GetStrobeWidth(width))
      throw ForwardException(fooName);

    return width;
  }

  void MotherboardAdapter::ConfigureStrobeFilter(int targetDuration, int durationThreshold)
  {
    static const char *fooName = "ConfigureStrobeFilter";

    CheckIntBoundaries<uint8_t>(targetDuration, fooName);
    CheckIntBoundaries<uint8_t>(durationThreshold, fooName);

    if (!Motherboard::ConfigureStrobeFilter(targetDuration, durationThreshold))
      throw ForwardException(fooName);
  }

  QueueInfo MotherboardAdapter::GetQueueInfo()
  {
    static const char *fooName = "GetQueueInfo";

    QueueInfo result;

    if (!Motherboard::GetQueueInfo(result))
      throw ForwardException(fooName);

    return result;
  }

  void MotherboardAdapter::SetQueuePeriod(int periodMs) {
    static const char* fooName = "SetQueuePeriod";

    CheckIntBoundaries<uint8_t>(periodMs, fooName);

    if (!Motherboard::SetQueuePeriod(periodMs))
      throw ForwardException(fooName);
  }
} // namespace Roki