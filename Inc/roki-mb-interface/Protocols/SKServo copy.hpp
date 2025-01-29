#pragma once
#include "SKSBase/SKSBase.hpp"
#include "roki-mb-interface/Motherboard.hpp"
#include <stdexcept>
#include <string>


namespace MbProtocols {
class SKServo {
private:
  class MBoardIOImpl {
  private:
    MbInterface::Motherboard *Mboard;

  public:
    MBoardIOImpl(MbInterface::Motherboard &mboard) : Mboard{&mboard} {}

    void Synchronize(const uint8_t *reqBuf, uint8_t reqSz, uint8_t *rspBuf,
                     uint8_t rspSz) {
      bool ok = Mboard->BodySendForward(reqBuf, reqSz, rspBuf, rspSz);
      if (!ok)
        throw std::runtime_error("Motherboard: " + Mboard->GetError());
    }
  };

private:
  MBoardIOImpl MBIO;
  StarkitServo::SKSBase Servo;

  mutable bool HasError = false;
  mutable std::string Error;

private:
  bool MakeError(const std::string &msg) const {
    HasError = true;
    Error = "SKServo: " + msg;
    return false;
  }

public:
  // Yup that's ugly but idk how to implement it prettier
  // in "we don't use exceptions" paradigm.
  // I suppose I would stick to template-based interface
  // until making a python adapter, like I did in Zubr class.
  template <typename Proc>
  bool Call(const typename Proc::Request &req, typename Proc::Request &rsp) {
    try {
      rsp = Servo.Call<Proc>(MBIO, req);
      return true;
    } catch (std::exception &e) {
      return MakeError(e.what());
    }
  }

  using Procedures = StarkitServo::SKSBase::Procedures;

public:
  SKServo(MbInterface::Motherboard &mboard) : MBIO{mboard} {}

  bool IsOk() const { return !HasError; }
  std::string GetError() const { return HasError ? Error : "Ok"; }

  SKServo(const SKServo &) = delete;
  SKServo &operator=(const SKServo &) = delete;

  SKServo(SKServo &&) = default;
  SKServo &operator=(SKServo &&) = default;
};
} // namespace MbProtocols
