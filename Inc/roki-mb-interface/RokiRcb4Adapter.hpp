#pragma once

#include "roki-mb-interface/RokiRcb4.hpp"
#include <tuple>

namespace MbProtocols {

class RokiRcb4Adapter : public RokiRcb4 {
public:
  RokiRcb4Adapter(MbInterface::Motherboard &mb);

public:
  bool setServoPos(std::vector<ServoData> &servoDatas, byte frame);
  bool setServoPosAsync(std::vector<ServoData> &servoDatas, byte frame,
                        byte pause = 0);
  bool setFreePos(std::vector<ServoData> &servoDatas);
  bool setHoldPos(std::vector<ServoData> &servoDatas);
  bool setServoSpeed(std::vector<ServoData> &servoDatas);
  bool setServoStretch(std::vector<ServoData> &servoDatas);
  std::tuple<bool, std::vector<byte>> moveRamToComCmdSynchronize(int addr,
                                                                 byte size);
};
} // namespace MbProtocols