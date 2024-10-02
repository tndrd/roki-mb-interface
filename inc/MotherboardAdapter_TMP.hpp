#pragma once

#include "MotherboardAdapter.hpp"

namespace Roki {

class MotherboardAdapter_TMP : public MotherboardAdapter {
public:
  MotherboardAdapter_TMP() = default;

  IMUFrame GetIMUFrame(int seq);

  FrameContainerInfo GetIMUContainerInfo();

  void ResetStrobeContainers();

  void SetIMUStrobeOffset(int offset);

  IMUFrame GetIMULatest();

  int GetStrobeWidth();
  void ConfigureStrobeFilter(int targetDuration, int durationThreshold);

  BodyQueueInfo GetBodyQueueInfo();
  void SetBodyQueuePeriod(int periodMs);
  void  ResetBodyQueue();

  Version GetVersion();
};

} // namespace Roki
