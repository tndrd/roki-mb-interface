#include "roki-mb-interface/ZubrAdapter.hpp"

namespace Roki {

ZubrAdapter::ZubrAdapter(Motherboard &mb) : Zubr{mb} {}

template <typename T> bool CheckIntBoundaries(int value) {
  if (value < std::numeric_limits<T>::min())
    return false;

  if (value > std::numeric_limits<T>::max())
    return false;

  return true;
}

#define INT_ERROR MakePrefixError(__func__, "Inappropriate integer value")

bool ZubrAdapter::MemISet(int addr, int value) {
  if (!CheckIntBoundaries<AddrT>(addr) || !CheckIntBoundaries<int32_t>(value))
    return INT_ERROR;

  return MemSet(addr, value);
}

bool ZubrAdapter::MemFSet(int addr, float value) {
  if (!CheckIntBoundaries<AddrT>(addr))
    return INT_ERROR;

  return MemSet(addr, value);
}

std::tuple<bool, int> ZubrAdapter::MemIGet(int addr) {
  if (!CheckIntBoundaries<AddrT>(addr))
    return {INT_ERROR, 0};

  int32_t value = 0;
  bool result = MemGet(addr, value);

  return {result, value};
}

std::tuple<bool, float> ZubrAdapter::MemFGet(int addr) {
  if (!CheckIntBoundaries<AddrT>(addr))
    return {INT_ERROR, 0};

  float value = 0;
  bool result = MemGet(addr, value);

  return {result, value};
}

#undef INT_ERROR

} // namespace Roki