#pragma once

#include "RokiRcb4.hpp"

namespace Roki
{

  class RokiRcb4Adapter : public RokiRcb4
  {
  public:
    RokiRcb4Adapter(Motherboard &mb);

  public:
    bool setServoPos(std::vector<ServoData> &servoDatas, byte frame);
    bool setServoPosAsync(std::vector<ServoData> &servoDatas, byte frame);
    bool setFreePos(std::vector<ServoData> &servoDatas);
    bool setHoldPos(std::vector<ServoData> &servoDatas);
    bool setServoSpeed(std::vector<ServoData> &servoDatas);
    bool setServoStretch(std::vector<ServoData> &servoDatas);
  };
} // namespace Roki