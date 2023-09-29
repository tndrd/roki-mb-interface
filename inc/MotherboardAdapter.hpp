#pragma once

#include "Motherboard.hpp"
#include <limits>
#include <tuple>

namespace Roki {

class MotherboardException : public std::runtime_error {
public:
  MotherboardException(const std::string &source, const std::string &msg);
};

class MotherboardAdapter : public Motherboard {
private:
  MotherboardException ForwardException(const std::string &source) const;

  template <typename T>
  void CheckIntBoundaries(int value, const char *fooName) const;

  template <typename T> using Ret = typename std::tuple<bool, T>;

public:
  MotherboardAdapter() = default;

  bool Configure(const TTYConfig &serviceConfig);

  Ret<IMUFrame> GetIMUFrame(int seq);
  Ret<std::vector<int>> GetBodyFrame(int seq);
  Ret<FrameContainerInfo> GetIMUContainerInfo();
  Ret<FrameContainerInfo> GetBodyContainerInfo();
  bool ResetStrobeContainers();
  bool SetIMUStrobeOffset(int offset);
  bool SetBodyStrobeOffset(int offset);
  Ret<IMUFrame> GetIMULatest();
  Ret<uint8_t> GetStrobeWidth();
  bool ConfigureStrobeFilter(int targetDuration, int durationThreshold);
  Ret<BodyQueueInfo> GetBodyQueueInfo();
  bool SetBodyQueuePeriod(int periodMs);
  Ret<Version> GetVersion();
};
} // namespace Roki