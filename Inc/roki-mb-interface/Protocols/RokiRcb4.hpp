#pragma once

#include "rcb4-base-class/Rcb4BaseClass.hpp"
#include "roki-mb-interface/Helpers/MbError.hpp"
#include "roki-mb-interface/Motherboard.hpp"

namespace MbProtocols {

class RokiRcb4 : public Rcb4BaseClass, public MbInterface::MbError {
private:
  // Hardcoded NACK sequence
  static constexpr uint8_t NACK[] = {0x4, 0xFE, 0x15, 0x17};

  // Hardcoded GetAllPostions request
  struct GetAllPosRequest {
    static constexpr uint8_t Data[] = {0xA, 0,    0x20, 0,    0,
                                       0,   0x70, 0,    0x1E, 0xB8};
    static constexpr uint8_t ReqSize = sizeof(Data);
    static constexpr uint8_t RspSize = 33;
  };

  struct UARTConfig {
    static constexpr size_t Baudrate = 1250000;
    static constexpr size_t ByteSize = 9;
    static constexpr size_t StopBits = 1;
    static constexpr size_t TimeoutMs = 200;
    static constexpr size_t ARQAttempts = 5;

    static constexpr size_t Parity =
        MbInterface::Messages::BodyUARTConfig::ParityVal::Even;
  };

private:
  MbInterface::Motherboard *Mboard;

public:
  RokiRcb4(MbInterface::Motherboard &mb);

private:
  virtual bool synchronize(byte txBuf[], byte txLen, byte rxBuf[],
                           byte rxLen) override;

  bool SendAsync(byte *txData, uint8_t pause);
  bool EnableARQ();
  bool ConfigureUart();

public:
  bool setServoPosAsync(ServoData servoDatas[], byte servoCount, byte frame,
                        uint8_t pause = 0);

  // overrides default motionPlay implementation
  bool motionPlay(int motionNum);

  bool moveRamToComCmdSynchronize(int addr, byte size, uint8_t *rxBuf);

  bool enableStrobeCallback();
};
} // namespace MbProtocols