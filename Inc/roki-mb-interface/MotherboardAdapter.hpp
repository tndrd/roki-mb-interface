#pragma once

#include "roki-mb-interface/Motherboard.hpp"
#include <limits>
#include <tuple>

namespace MbInterface {

class MotherboardAdapter : public Motherboard {
private:
  template <typename T> bool CheckIntBoundaries(int value);

  template <typename T> using Ret = typename std::tuple<bool, T>;
  using Buffer = std::vector<uint8_t>;

public:
  MotherboardAdapter() = default;

  Ret<IMUFrame> GetIMUFrame(int seq);
  Ret<Buffer> GetBodyFrame(int seq);

  Ret<FrameContainerInfo> GetIMUContainerInfo();
  Ret<FrameContainerInfo> GetBodyContainerInfo();

  bool SetIMUStrobeOffset(int offset);
  bool SetBodyStrobeOffset(int offset);

  Ret<IMUFrame> GetIMULatest();

  Ret<int> GetStrobeWidth();
  bool ConfigureStrobeFilter(int targetDuration, int durationThreshold);

  Ret<BodyQueueInfo> GetBodyQueueInfo();
  bool SetBodyQueuePeriod(int periodMs);

  Ret<Version> GetVersion();

  bool SetBodyTimeout(int timeoutMs);

  virtual ~MotherboardAdapter() = default;

  MotherboardAdapter(const MotherboardAdapter &) = delete;
  MotherboardAdapter &operator=(const MotherboardAdapter &) = delete;

  MotherboardAdapter(MotherboardAdapter &&) = default;
  MotherboardAdapter &operator=(MotherboardAdapter &&) = default;
};

} // namespace MbInterface