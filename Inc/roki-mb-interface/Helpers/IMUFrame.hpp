#pragma once
#include "roki-mb-service/MbMessages.hpp"

namespace MbInterface {
namespace Helpers {
struct IMUFrame {
public:
  struct Quaternion {
    float X;
    float Y;
    float Z;
    float W;
  };

  using TimestampT = Messages::IMUFrameMsg::TimestampT;

private:
  static float NormalizeCoord(int16_t crd);

  static Quaternion
  NormalizeQuaternion(const Messages::IMUFrameMsg::QuaternionMsg &msg);

public:
  Quaternion Orientation;
  Messages::IMUFrameMsg::TimestampT Timestamp;
  uint8_t SensorID;

public:
  static IMUFrame ConvertFrom(const Messages::IMUFrameMsg &msg);
};

} // namespace Helpers
} // namespace MbInterface