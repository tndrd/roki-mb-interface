#include "Helpers/IMUFrame.hpp"

namespace Roki {
namespace Helpers {

float IMUFrame::NormalizeCoord(int16_t crd) {
  static const size_t normFactor = 16384; // 2*14
  return float(crd) / normFactor;
}

IMUFrame::Quaternion
IMUFrame::NormalizeQuaternion(const Messages::IMUFrameMsg::QuaternionMsg &msg) {
  Quaternion qt;

  qt.X = NormalizeCoord(msg.X);
  qt.Y = NormalizeCoord(msg.Y);
  qt.Z = NormalizeCoord(msg.Z);
  qt.W = NormalizeCoord(msg.W);

  return qt;
}

IMUFrame IMUFrame::ConvertFrom(const Messages::IMUFrameMsg &msg) {
  IMUFrame frame;

  frame.Timestamp = msg.Timestamp;
  frame.SensorID = msg.SensorID;
  frame.Orientation = NormalizeQuaternion(msg.Orientation);

  return frame;
}

} // namespace Helpers
} // namespace Roki