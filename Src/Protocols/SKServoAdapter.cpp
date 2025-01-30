#pragma once
#include "roki-mb-interface/Protocols/SKServoAdapter.hpp"

namespace MbProtocols {

using P = SKServo::Procedures;

template <typename T>
bool SKServoAdapter::CheckParam(int param, const char *pname) {
  T min = std::numeric_limits<T>::min();
  T max = std::numeric_limits<T>::max();

  if (param >= min && param <= max)
    return MakeSuccess();

  return MakeError(std::string{"Parameter \""} + pname +
                   "\" doesn't fit in corresponding C++ type");
}

#define RSPTPL(proc) std::tuple<bool, P::proc::Responce>

#define VALIDATE(param, type)                                                  \
  if (!CheckParam<type>(param), #param)                                        \
    return {false, {}};
#define CHECK_ID VALIDATE(id, uint8_t)

#define CALL(proc, ...)                                                        \
  P::proc::Responce rsp;                                                       \
  bool ok = Call<P::proc>({__VA_ARGS__}, rsp);                                 \
  return {ok, rsp};

RSPTPL(Control) SKServoAdapter::SetPosition(int id, int position) {
  CHECK_ID;
  VALIDATE(position, int16_t);
  CALL(Control, id, position);
}

RSPTPL(Info) SKServoAdapter::GetInfo(int id) {
  CHECK_ID;
  CALL(Info, id);
}

RSPTPL(Read) SKServoAdapter::GetParam(int id, int index) {
  CHECK_ID;
  VALIDATE(index, int16_t);
  CALL(Read, id, index);
}

RSPTPL(Write) SKServoAdapter::SetParam(int id, int index, int value) {
  CHECK_ID;
  VALIDATE(index, int16_t);
  VALIDATE(value, int32_t);
  CALL(Write, id, index, value);
}

} // namespace MbProtocols
