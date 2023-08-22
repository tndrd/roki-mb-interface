#include "IMUInterface.hpp"
namespace Roki {

IMUInterface::IMUFrame
IMUInterface::Deserialize(const MBInterface::InPackage &package) {
  assert(package.ResponceSize == ResponceSize);

  IMUFrame fr;
  // cppcheck-suppress uninitStructMember
  auto &qt = fr.Orientation;

  // cppcheck-suppress uninitStructMember
  auto &ts = fr.Timestamp;

  const uint8_t *ptr = package.Data;

  qt.X = *reinterpret_cast<const int16_t *>(ptr);
  ptr += sizeof(int16_t);

  qt.Y = *reinterpret_cast<const int16_t *>(ptr);
  ptr += sizeof(int16_t);

  qt.Z = *reinterpret_cast<const int16_t *>(ptr);
  ptr += sizeof(int16_t);

  qt.W = *reinterpret_cast<const int16_t *>(ptr);
  ptr += sizeof(int16_t);

  qt.Accuracy = *reinterpret_cast<const float *>(ptr);
  ptr += sizeof(float);

  ts.TimeS = *reinterpret_cast<const uint32_t *>(ptr);
  ptr += sizeof(uint32_t);

  ts.TimeS = *reinterpret_cast<const uint32_t *>(ptr);
  ptr += sizeof(uint32_t);

  fr.SensorID = *reinterpret_cast<const uint8_t *>(ptr);
  ptr += sizeof(uint8_t);

  return fr;
}

bool IMUInterface::IsOk() const { return !HasError; }
std::string IMUInterface::GetError() const { return Error; }

bool IMUInterface::GetFrame(MBInterface &mbi, uint16_t seq, IMUFrame &dest) {
  std::array<uint8_t, 2> buf{0, 0};
  *reinterpret_cast<uint16_t *>(buf.data()) = seq;

  MBInterface::OutPackage outPackage{MBInterface::Periphery::Imu, ResponceSize,
                                     0, buf.data(), buf.size()};

  if (!mbi.Send(outPackage)) {
    HasError = true;
    Error = mbi.GetError();
    return false;
  }

  MBInterface::InPackage inPackage;
  if (!mbi.Receive(ResponceSize, inPackage)) {
    HasError = true;
    Error = mbi.GetError();
    return false;
  }

  if (inPackage.Error != 0) {
    HasError = true;
    Error = "Frame " + std::to_string(seq) + " is unavailable";
    return false;
  }

  dest = Deserialize(inPackage);
  return true;
}
} // namespace Roki