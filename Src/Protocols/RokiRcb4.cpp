#include "roki-mb-interface/Protocols/RokiRcb4.hpp"

namespace MbProtocols {

RokiRcb4::RokiRcb4(MbInterface::Motherboard &mb)
    : Mboard{&mb}, MbError{"Rcb4"} {
  if (ConfigureUart() && EnableARQ())
    return;

  throw std::runtime_error("Failed to create Rcb4 instance: " + GetError());
}

bool RokiRcb4::synchronize(byte txBuf[], byte txLen, byte rxBuf[], byte rxLen) {
  if (!Mboard->BodySendForward(txBuf, txLen, rxBuf, rxLen))
    return MakeError("Failed to synchronize: " + Mboard->GetError());

  return MakeSuccess();
}

bool RokiRcb4::SendAsync(byte *txData, uint8_t pause) {
  uint8_t txSize = *txData;

  if (!Mboard->BodySendQueue(txData, txSize, 4, pause))
    return MakeError("Failed to send async: " + Mboard->GetError());

  return MakeSuccess();
}

bool RokiRcb4::setServoPosAsync(ServoData servoDatas[], byte servoCount,
                                byte frame, uint8_t pause) {
  if (runConstFrameServoCmd(servoDatas, servoCount, frame) == -1)
    return MakeError("Failed to set servo pos: ill command");
  return SendAsync(txBuf, pause);
}

bool RokiRcb4::motionPlay(int motionNum) {
  setMotionNum(motionNum);
  return resume() ? MakeSuccess() : MakeError("Failed to play motion");
}

bool RokiRcb4::moveRamToComCmdSynchronize(int addr, byte size, uint8_t *rxBuf) {
  bool res =
      Rcb4BaseClass::moveRamToComCmdSynchronize(addr, size, rxBuf) == size;
  return res ? MakeSuccess() : MakeError("Failed to read RAM");
}

bool RokiRcb4::EnableARQ() {
  bool res = Mboard->EnableBodyARQ(NACK, sizeof(NACK), UARTConfig::ARQAttempts);

  return res ? MakeSuccess()
             : MakeError("Failed to enable ARQ: " + Mboard->GetError());
}

bool RokiRcb4::ConfigureUart() {
  MbInterface::Messages::BodyUARTConfig config;

  config.Baudrate.Value = UARTConfig::Baudrate;
  config.ByteSize.Value = UARTConfig::ByteSize;
  config.Parity.Value = UARTConfig::Parity;
  config.StopBits.Value = UARTConfig::StopBits;
  config.TimeoutMs.Value = UARTConfig::TimeoutMs;

  bool ok = Mboard->ConfigureBodyUART(config);

  return ok ? MakeSuccess()
            : MakeError("Failed to configure Uart: " + Mboard->GetError());
}

bool RokiRcb4::enableStrobeCallback() {
  using Req = GetAllPosRequest;
  bool res =
      Mboard->SetBodyStrobeCallback(Req::Data, Req::ReqSize, Req::RspSize);

  return res ? MakeSuccess()
             : MakeError("Failed to enable strobe callback: " +
                         Mboard->GetError());
}

} // namespace MbProtocols