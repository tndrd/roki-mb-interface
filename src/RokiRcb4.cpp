#include "RokiRcb4.hpp"

namespace Roki
{

  bool RokiRcb4::MakeError(const std::string &msg)
  {
    HasError = true;
    Error = "Rcb4: " + msg;
    return false;
  }

  RokiRcb4::RokiRcb4(Motherboard &mb) : Mboard(&mb) { assert(Mboard); }
  bool RokiRcb4::IsOk() const { return !HasError; }

  std::string RokiRcb4::GetError() const
  {
    return HasError ? Error : "Internal rcb4 error";
  }

  bool RokiRcb4::synchronize(byte txBuf[], byte txLen, byte rxBuf[], byte rxLen)
  {
    if (!Mboard->BodySendSync(txBuf, txLen, rxBuf, rxLen))
      return MakeError("Failed to synchronize: " + Mboard->GetError());

    HasError = false;
    return true;
  }

  bool RokiRcb4::SendAsync(byte *txData, uint8_t nPause)
  {
    uint8_t txSize = *txData;

    if (!Mboard->BodySendAsync(txData, txSize, 4, nPause))
      return MakeError("Failed to send async: " + Mboard->GetError());

    HasError = false;
    return true;
  }

  bool RokiRcb4::setServoPosAsync(ServoData servoDatas[], byte servoCount,
                                  byte frame, uint8_t nPause)
  {
    if (runConstFrameServoCmd(servoDatas, servoCount, frame) == -1)
      return MakeError("Failed to set servo pos: ill command");
    return SendAsync(txBuf, nPause);
  }

  bool RokiRcb4::motionPlay(int motionNum)
  {
    return setMotionNum(motionNum);
    //return resume();
  }

  bool	RokiRcb4::moveRamToComCmdSynchronize(int addr, byte size, uint8_t* rxBuf) {
    return Rcb4BaseClass::moveRamToComCmdSynchronize(addr, size, rxBuf) == size;
  }

} // namespace Roki
