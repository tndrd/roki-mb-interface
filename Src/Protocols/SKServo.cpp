#include "roki-mb-interface/Protocols/SKServo.hpp"

#ifdef USE_MB_MOCK
#pragma message "USE_MB_MOCK is set, building using SKServo mock"
#error "SKServo mock is not yet implemented"
#endif

namespace MbProtocols {

SKServo::MBoardIOImpl::MBoardIOImpl(MbInterface::Motherboard &mboard)
    : Mboard{&mboard} {}

void SKServo::MBoardIOImpl::Synchronize(const uint8_t *reqBuf, uint8_t reqSz,
                                        uint8_t *rspBuf, uint8_t rspSz) {
  bool ok = Mboard->BodySendForward(reqBuf, reqSz, rspBuf, rspSz);
  if (!ok)
    throw std::runtime_error("Motherboard: " + Mboard->GetError());
}

SKServo::SKServo(MbInterface::Motherboard &mboard)
    : MBIO{mboard}, MbError{"SKServo"} {
  if (!ConfigureUart())
    throw std::runtime_error("Failed to configure Uart: " + GetError());
}

bool SKServo::ConfigureUart() {
  MbInterface::Messages::BodyUARTConfig config;

  config.Baudrate.Value = UARTConfig::Baudrate;
  config.ByteSize.Value = UARTConfig::ByteSize;
  config.Parity.Value = UARTConfig::Parity;
  config.StopBits.Value = UARTConfig::StopBits;
  config.TimeoutMs.Value = UARTConfig::TimeoutMs;

  bool ok = MBIO.Mboard->ConfigureBodyUART(config);

  return ok ? MakeSuccess()
            : MakeError("Failed to configure Uart: " + MBIO.Mboard->GetError());
}

} // namespace MbProtocols
