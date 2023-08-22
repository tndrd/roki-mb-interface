#pragma once

#include "MBInterface.hpp"

namespace Roki {
class IMUInterface {
public:
  struct TimestampT {
    size_t TimeS;
    size_t TimeNS;
  };

  struct Quaternion {
    int16_t X;
    int16_t Y;
    int16_t Z;
    int16_t W;

    //float Accuracy;
  };

  struct IMUFrame {
    Quaternion Orientation;
    TimestampT Timestamp;

    uint8_t SensorID;
  };

private:
  static constexpr uint8_t ResponceSize = 4 * sizeof(int16_t) + /* sizeof(float) + */
                                          2 * sizeof(uint32_t) +
                                          sizeof(uint8_t);

  static constexpr uint8_t RequestSize = 2;

  bool HasError = false;
  std::string Error;

  IMUFrame Deserialize(const MBInterface::InPackage &package);

public:
  bool IsOk() const;
  std::string GetError() const;
  bool GetFrame(MBInterface &mbi, uint16_t seq, IMUFrame &dest);
};

} // namespace Roki