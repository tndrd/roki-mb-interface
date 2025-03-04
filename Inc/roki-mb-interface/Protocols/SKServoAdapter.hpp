#pragma once
#include "roki-mb-interface/Protocols/SKServo.hpp"
#include <limits>
#include <tuple>

namespace MbProtocols {

class SKServoAdapter final : public SKServo {
public:
  using Params = SKServo::Params;

private:
  template <typename T> bool CheckParam(int param, const char *pname);
  using P = SKServo::Procedures;

public:
#define RSPTPL(proc) std::tuple<bool, P::proc::Responce>

  RSPTPL(Control) SetPosition(int id, int position);
  RSPTPL(Control) SetFree(int id);
  RSPTPL(Control) SetHold(int id);
  RSPTPL(Control) SetSoft(int id);

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
