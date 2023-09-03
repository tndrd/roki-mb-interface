#pragma once

#include "Motherboard.hpp"
#include "Rcb4BaseClass.hpp"

namespace Roki {

class RokiRcb4 : public Rcb4BaseClass {
private:
  Motherboard *Mboard;

  bool HasError = false;
  std::string Error;

private:
  bool MakeError(const std::string &msg);

public:
  RokiRcb4(Motherboard &mb);
  bool IsOk() const;
  std::string GetError() const;

private:
  virtual bool synchronize(byte txBuf[], byte txLen, byte rxBuf[],
                           byte rxLen) override;

  bool SendAsync(byte *txData);

public:
  bool setServoPosAsync(ServoData servoDatas[], byte servoCount, byte frame);
  
  // overrides default motionPlay implementation
  bool motionPlay(int motionNum);
};
} // namespace Roki