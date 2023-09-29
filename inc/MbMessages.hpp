#pragma once

#include <array>
#include <assert.h>
#include <memory.h>

namespace Roki {
namespace Messages {

struct IMUFrame {
  struct TimestampT {
    uint32_t TimeS;
    uint32_t TimeNS;
  };

  struct Quaternion {
    float X;
    float Y;
    float Z;
    float W;
  };

  Quaternion Orientation;
  TimestampT Timestamp;

  uint8_t SensorID;

  static constexpr size_t Size = 4 * sizeof(int16_t) + /* sizeof(float) + */
                                 2 * sizeof(uint32_t) + sizeof(uint8_t);

  static IMUFrame Deserialize(const uint8_t *ptr, size_t size);
};

struct FrameContainerInfo {
  uint16_t First;
  uint16_t NumAv;
  uint16_t MaxFrames;

  static constexpr size_t Size = 3 * sizeof(uint16_t);
  static FrameContainerInfo Deserialize(const uint8_t *ptr, size_t size);
};

struct Empty {
  static constexpr size_t Size = 1;

  void Serialize(uint8_t *ptr) const;
  static Empty Deserialize(const uint8_t *ptr, size_t size);
};

struct Byte {
  static constexpr size_t Size = 1;
  uint8_t Value;

  void Serialize(uint8_t *ptr) const;
  static Byte Deserialize(const uint8_t *ptr, size_t size);
};

struct FrameNumber {
  static constexpr size_t Size = 2;
  uint16_t Seq;

  void Serialize(uint8_t *ptr) const;
};

struct StrobeFilterConfig {
  static constexpr size_t Size = 2;
  uint8_t TargetDuration;
  uint8_t DurationThreshold;

  void Serialize(uint8_t *ptr) const;
};

struct Version {
  static constexpr size_t Size = 2;
  uint8_t Major;
  uint8_t Minor;

  static Version Deserialize(const uint8_t *ptr, size_t size);
};

struct BodyQueueInfo {
  static constexpr size_t Size = 2 * sizeof(uint16_t);

  uint16_t NumRequests;
  uint16_t NumResponces;

  static BodyQueueInfo Deserialize(const uint8_t *ptr, size_t size);
};

struct PeriodMs {
  static constexpr size_t Size = 1;

  uint8_t Ms;

  void Serialize(uint8_t *ptr) const;
};

struct BodyRequest {
  const uint8_t *Data;
  uint8_t Size;

  uint8_t ResponceSize;

  void Serialize(uint8_t *ptr) const;
};

struct BodyResponce {
  const uint8_t *Data;
  uint8_t Size;

  static BodyResponce Deserialize(const uint8_t *ptr, size_t size);
};

} // namespace Messages
} // namespace Roki