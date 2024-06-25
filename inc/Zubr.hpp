#pragma once

#include "Motherboard.hpp"
#include <array>
#include <numeric>

namespace Roki {

class Zubr {
public:
  using AddrT = uint16_t;
  static constexpr size_t DataWidth = 4;

private:
  static constexpr uint8_t GetCmdID = 0x21;
  static constexpr uint8_t GetCmdReqSz = 5;
  static constexpr uint8_t GetCmdRspSz = 9;

  static constexpr uint8_t SetCmdID = 0x31;
  static constexpr uint8_t SetCmdReqSz = 9;

  static constexpr std::array<uint8_t, 4> SetCmdAck{0x4, 0xfe, 0x6, 0x8};
  static constexpr uint8_t SetCmdRspSz = SetCmdAck.size();

  static constexpr size_t BufSize = 16;

  Motherboard *Mboard;
  std::array<uint8_t, BufSize> Buffer;

  mutable bool HasError = false;
  mutable std::string Error;

protected:
  bool MakeError(const std::string &msg) const;
  bool MakePrefixError(const std::string &prefix, const std::string &msg) const;

public:
  Zubr(Motherboard &mb);

  bool IsOk() const;
  std::string GetError() const;

private:
  uint8_t EvalCheckSum() const;
  bool Synchronize(uint8_t responceSize);

  void PackGetCmd(AddrT addr);
  bool UnPackGetCmd(AddrT addr, const uint8_t **dataPtr) const;

  void PackSetCmd(AddrT addr, const uint8_t *data);
  bool UnPackSetCmd() const;


public:
  template <typename ValueT> bool MemSet(AddrT addr, ValueT value) {
    static_assert(sizeof(ValueT) == DataWidth);

    PackSetCmd(addr, reinterpret_cast<const uint8_t *>(&value));
    if (!Synchronize(SetCmdRspSz))
      return false;

    return UnPackSetCmd();
  }

  template <typename ValueT> bool MemGet(AddrT addr, ValueT &value) {
    static_assert(sizeof(ValueT) == DataWidth);

    const uint8_t *data = nullptr;

    PackGetCmd(addr);

    if (!Synchronize(GetCmdRspSz))
      return false;
    if (!UnPackGetCmd(addr, &data))
      return false;

    assert(data);
    value = *reinterpret_cast<const ValueT *>(data);

    return true;
  }
}; // namespace Roki

} // namespace Roki