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

bool SKServo::MakeError(const std::string &msg) const {
  HasError = true;
  Error = "SKServo: " + msg;
  return false;
}

SKServo::SKServo(MbInterface::Motherboard &mboard) : MBIO{mboard} {}

bool SKServo::IsOk() const { return !HasError; }
std::string SKServo::GetError() const { return HasError ? Error : "Ok"; }

} // namespace MbProtocols
