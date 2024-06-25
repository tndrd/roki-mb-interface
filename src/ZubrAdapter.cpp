#include "ZubrAdapter.hpp"

namespace Roki {

ZubrAdapter::ZubrAdapter(Motherboard &mb) : Zubr{mb} {}

#define DEFINE_SETTER(name, type)                                              \
  bool ZubrAdapter::name(AddrT addr, type value) { return MemSet(addr, value); }

#define DEFINE_GETTER(name, type)                                              \
  std::tuple<bool, type> ZubrAdapter::name(AddrT addr) {                       \
    type value{};                                                              \
    bool result = MemGet(addr, value);                                         \
                                                                               \
    return {result, value};                                                    \
  }

DEFINE_SETTER(MemISet, int)
DEFINE_SETTER(MemFSet, float)

DEFINE_GETTER(MemIGet, int)
DEFINE_GETTER(MemFGet, float)

#undef DEFINE_GETTER
#undef DEFINE_SETTER

} // namespace Roki