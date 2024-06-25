#pragma once

#include "Zubr.hpp"
#include <tuple>

namespace Roki {

class ZubrAdapter : public Zubr {
public:
  ZubrAdapter(Motherboard &mb);

  bool MemISet(AddrT addr, int32_t value);
  bool MemFSet(AddrT addr, float value);

  std::tuple<bool, int32_t> MemIGet(AddrT addr);
  std::tuple<bool, float> MemFGet(AddrT addr);
};

} // namespace Roki