#include "roki-mb-interface/Zubr.hpp"

namespace MbProtocols {

bool Zubr::MakeError(const std::string &msg) const {
  HasError = true;
  Error = "Zubr: " + msg;
  return false;
}

bool Zubr::MakePrefixError(const std::string &prefix,
                           const std::string &msg) const {
  return MakeError(prefix + ": " + msg);
}

#define FOO_ERROR(msg) MakePrefixError(__func__, msg)

Zubr::Zubr(MbInterface::Motherboard &mb) : Mboard{&mb} {}

bool Zubr::IsOk() const { return HasError; }
std::string Zubr::GetError() const { return HasError ? Error : "No error"; }

uint8_t Zubr::EvalCheckSum() const {
  uint8_t len = Buffer[0];
  return std::accumulate(&Buffer[0], &Buffer[len - 1], 0);
}

void Zubr::PackGetCmd(AddrT addr) {
  Buffer[0] = GetCmdReqSz;
  Buffer[1] = GetCmdID;
  *reinterpret_cast<AddrT *>(&Buffer[2]) = addr;
  Buffer[GetCmdReqSz - 1] = EvalCheckSum();
}

bool Zubr::UnPackGetCmd(AddrT addr, const uint8_t **dataPtr) const {
  assert(dataPtr);

  if (Buffer[0] != GetCmdRspSz)
    return FOO_ERROR("Wrong responce size");
  if (Buffer[1] != SetCmdID)
    return FOO_ERROR("Wrong responce ID");

  AddrT rxAddr = *reinterpret_cast<const AddrT *>(&Buffer[2]);

  if (rxAddr != addr)
    return FOO_ERROR("Request and responce addresses are not equal");
  if (Buffer[GetCmdRspSz - 1] != EvalCheckSum())
    return FOO_ERROR("Bad checksum");

  *dataPtr = &Buffer[4];

  return true;
}

void Zubr::PackSetCmd(AddrT addr, const uint8_t *data) {
  assert(data);

  Buffer[0] = SetCmdReqSz;
  Buffer[1] = SetCmdID;
  *reinterpret_cast<AddrT *>(&Buffer[2]) = addr;

  memcpy(&Buffer[4], data, DataWidth);

  Buffer[SetCmdReqSz - 1] = EvalCheckSum();
}

bool Zubr::UnPackSetCmd() const {
  if (memcmp(&Buffer[0], &SetCmdAck[0], SetCmdAck.size()) != 0)
    return FOO_ERROR("Bad responce");

  return true;
}

bool Zubr::Synchronize(uint8_t responceSize) {
  bool ret = Mboard->BodySendForward(Buffer.data(), Buffer[0], Buffer.data(),
                                     responceSize);
  if (!ret)
    return FOO_ERROR(Mboard->GetError());

  return true;
}

#undef FOO_ERROR

} // namespace MbProtocols
