#pragma once

#include "roki-mb-interface/Protocols/Zubr.hpp"
#include <tuple>

namespace MbProtocols {

class ZubrAdapter : public Zubr {
public:
  ZubrAdapter(MbInterface::Motherboard &mb);

  bool MemISet(int addr, int value);
  bool MemFSet(int addr, float value);

  std::tuple<bool, int> MemIGet(int addr);
  std::tuple<bool, float> MemFGet(int addr);
};

} // namespace MbProtocols