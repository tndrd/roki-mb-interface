#pragma once
#include "roki-mb-interface/Protocols/SKServo.hpp"
#include <limits>
#include <tuple>

namespace MbProtocols {

class SKServoAdapter : public SKServo {
private:
  template <typename T> bool CheckParam(int param, const char *pname);

public:
#define RSPTPL(proc) std::tuple<bool, P::proc::Responce>

  RSPTPL(Control) SetPosition(int id, int position);
  RSPTPL(Info) GetInfo(int id);
  RSPTPL(Read) GetParam(int id, int index);
  RSPTPL(Write) SetParam(int id, int index, int value);

#undef RSPTPL

public:
  SKServoAdapter(MbInterface::Motherboard &mboard) : SKServo{mboard} {}

  SKServoAdapter(const SKServo &) = delete;
  SKServoAdapter &operator=(const SKServoAdapter &) = delete;

  SKServoAdapter(SKServoAdapter &&) = default;
  SKServoAdapter &operator=(SKServoAdapter &&) = default;
};
} // namespace MbProtocols
