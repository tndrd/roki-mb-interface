#include "MbDefaultConfig.hpp"

namespace Roki {

bool MbDefaultConfig(Motherboard &mb) {
  Roki::TTYConfig conf;

  conf.Port = "/dev/ttyAMA2";
  conf.Baudrate = 1250000;
  conf.ParityBit = true;
  conf.Stopbits = Roki::TTYConfig::StopbitsCount::One;
  conf.Timeout = 1;

  return mb.Configure(conf);
}

} // namespace Roki
