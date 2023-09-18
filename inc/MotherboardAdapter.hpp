#pragma once

#include "Motherboard.hpp"
#include "MotherboardAliases.hpp"

#include <stdexcept>
#include <limits>

namespace Roki {

class MotherboardException : public std::runtime_error {
public:
  MotherboardException(const std::string &source, const std::string &msg);
};

class MotherboardAdapter : public Motherboard {
private:
  MotherboardException ForwardException(const std::string &source) const;
  
  template<typename T>
  void CheckIntBoundaries(int value, const char* fooName) const;

public:
  MotherboardAdapter() = default;

  void Configure(const SerialInterface::TTYConfig &serviceConfig);

  IMUFrame GetOrientationBySeq(int seq);
  IMUFrame GetCurrentOrientation();
  IMUInfo GetIMUInfo();
  void ResetIMUCounter();
  void SetStrobeOffset(int offset);
  int GetStrobeWidth();
  void ConfigureStrobeFilter(int targetDuration, int durationThreshold);

  QueueInfo GetQueueInfo();
  void SetQueuePeriod(int periodMs);
};
} // namespace Roki