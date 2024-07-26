#pragma once

#include "roki-mb-interface/Motherboard.hpp"
#include <limits>
#include <tuple>

namespace MbInterface {

class MotherboardAdapter : public Motherboard {
private:
  bool MakeError(const std::string &msg);

  template <typename T> bool CheckIntBoundaries(int value);

  template <typename T> using Ret = typename std::tuple<bool, T>;

public:
  MotherboardAdapter() = default;

  Ret<IMUFrame> GetIMUFrame(int seq);
  Ret<std::vector<uint8_t>> GetBodyFrame(int seq);

  Ret<FrameContainerInfo> GetIMUContainerInfo();
  Ret<FrameContainerInfo> GetBodyContainerInfo();

  bool ResetStrobeContainers();

  bool SetIMUStrobeOffset(int offset);
  bool SetBodyStrobeOffset(int offset);

  Ret<IMUFrame> GetIMULatest();

  Ret<int> GetStrobeWidth();
  bool ConfigureStrobeFilter(int targetDuration, int durationThreshold);

  Ret<BodyQueueInfo> GetBodyQueueInfo();
  bool SetBodyQueuePeriod(int periodMs);
  bool ResetBodyQueue();

  Ret<Version> GetVersion();

  virtual ~MotherboardAdapter() = default;

  MotherboardAdapter(const MotherboardAdapter &) = delete;
  MotherboardAdapter &operator=(const MotherboardAdapter &) = delete;

  MotherboardAdapter(MotherboardAdapter &&) = default;
  MotherboardAdapter &operator=(MotherboardAdapter &&) = default;
};

} // namespace MbInterface