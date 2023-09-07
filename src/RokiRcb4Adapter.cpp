#include "RokiRcb4Adapter.hpp"

namespace Roki
{

  RokiRcb4Adapter::RokiRcb4Adapter(Motherboard &mb) : RokiRcb4{mb} {}

  bool RokiRcb4Adapter::setServoPos(std::vector<ServoData> &servoDatas, byte frame)
  {
    return RokiRcb4::setServoPos(servoDatas.data(), servoDatas.size(), frame);
  }

  bool RokiRcb4Adapter::setServoPosAsync(std::vector<ServoData> &servoDatas, byte frame)
  {
    return RokiRcb4::setServoPosAsync(servoDatas.data(), servoDatas.size(), frame);
  }

  bool RokiRcb4Adapter::setFreePos(std::vector<ServoData> &servoDatas)
  {
    return RokiRcb4::setFreePos(servoDatas.data(), servoDatas.size());
  }

  bool RokiRcb4Adapter::setHoldPos(std::vector<ServoData> &servoDatas)
  {
    return RokiRcb4::setHoldPos(servoDatas.data(), servoDatas.size());
  }

  bool RokiRcb4Adapter::setServoSpeed(std::vector<ServoData> &servoDatas)
  {
    return RokiRcb4::setServoSpeed(servoDatas.data(), servoDatas.size());
  }

  bool RokiRcb4Adapter::setServoStretch(std::vector<ServoData> &servoDatas)
  {
    return RokiRcb4::setServoStretch(servoDatas.data(), servoDatas.size());
  }

  std::tuple<bool, std::vector<byte>> RokiRcb4Adapter::moveRamToComCmdSynchronize(int addr, byte size)
  {
    std::vector<byte> rxData(size, 0);
    if (RokiRcb4::moveRamToComCmdSynchronize(addr, size, rxData.data()))
      return {true, rxData};
    else
      return {false, rxData};
  }

} // namespace Roki