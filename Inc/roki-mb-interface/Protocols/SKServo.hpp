#pragma once
#include "SKSBase/SKSBase.hpp"
#include "roki-mb-interface/Helpers/MbError.hpp"
#include "roki-mb-interface/Motherboard.hpp"
#include <stdexcept>
#include <string>

namespace MbProtocols {
class SKServo : public MbInterface::MbError {
public:
  using Procedures = StarkitServo::SKSBase::Procedures;
  using Params = StarkitServo::SKSBase::Params;
  using Limits = StarkitServo::SKSBase::Limits;

private:
  struct UARTConfig {
    static constexpr size_t Baudrate = 3000000;
    static constexpr size_t ByteSize = 8;
    static constexpr size_t StopBits = 1;
    static constexpr size_t TimeoutMs = 200;

    static constexpr size_t Parity =
        MbInterface::Messages::BodyUARTConfig::ParityVal::None;
  };

private:
  class MBoardIOImpl {
  public:
    MbInterface::Motherboard *Mboard;

  public:
    MBoardIOImpl(MbInterface::Motherboard &mboard);

    void Synchronize(const uint8_t *reqBuf, uint8_t reqSz, uint8_t *rspBuf,
                     uint8_t rspSz);
  };

private:
  MBoardIOImpl MBIO;
  StarkitServo::SKSBase Servo;

private:
  bool ConfigureUart();

public:
  // Yup that's ugly but idk how to implement it prettier
  // in "we don't use exceptions" paradigm.
  // I suppose I would stick to template-based interface
  // until making a python adapter, like I did in Zubr class.
  template <typename Proc>
  bool Call(const typename Proc::Request &req, typename Proc::Responce &rsp) {
    try {
      rsp = Servo.Call<Proc>(MBIO, req);
      return MakeSuccess();
    } catch (std::exception &e) {
      return MakeError(e.what());
    }
  }

public:
  SKServo(MbInterface::Motherboard &mboard);

  SKServo(const SKServo &) = delete;
  SKServo &operator=(const SKServo &) = delete;

  SKServo(SKServo &&) = default;
  SKServo &operator=(SKServo &&) = default;

  virtual ~SKServo() = default;
};
} // namespace MbProtocols
