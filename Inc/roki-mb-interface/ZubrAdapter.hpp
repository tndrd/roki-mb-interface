#pragma once

#include "roki-mb-interface/Zubr.hpp"
#include <tuple>

namespace Roki {

class ZubrAdapter : public Zubr {
public:
  ZubrAdapter(Motherboard &mb);

  bool MemISet(int addr, int value);
  bool MemFSet(int addr, float value);

  std::tuple<bool, int> MemIGet(int addr);
  std::tuple<bool, float> MemFGet(int addr);
};

} // namespace Roki