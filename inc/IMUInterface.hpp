#include "MotherboardInterface.hpp"

namespace Roki {

struct TimestampT {
  size_t TimeS;
  size_t TimeNS;
};

struct QuaternionT {
  int X;
  int Y;
  int W;
  int H;

  float Accuracy;
};

struct IMUFrame {
  QuaternionT Quaternion;
  TimestampT Timestamp;

  uint8_t SensorID;
};

class IMUInterface {

  static constexpr uint8_t ResponceSize = 4 * sizeof(int16_t) + sizeof(float) +
                                          2 * sizeof(uint32_t) +
                                          sizeof(uint8_t);

  static constexpr uint8_t RequestSize = 2;

  bool HasError;
  std::string Error;

  QuaternionT Deserialize(const InPackage& package) {
    
  }

  bool GetFrame(uint16_t seq, MBInterface &mbi, IMUFrame &dest) {
    std::vector<uint8_t> buf {0, 0};
    *reinterpret_cast<uint16_t*>(buf.data()) = seq;
    
    OutPackage outPackage { Periphery::Imu, ResponceSize, 0, std::move(buf)};

    if (!mbi.Send(outPackage)) {
      HasError = true;
      Error = mbi.GetError();
      return false;
    }

    InPackage inPackage;
    if (!mbi.Receive(ResponceSize, inPackage)) {
      HasError = true;
      Error = mbi.GetError();
      return false;
    }

    auto& qt = dest.Quaternion;
  }
};

} // namespace Roki