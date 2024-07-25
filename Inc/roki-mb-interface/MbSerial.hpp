#pragma once

#include <asm/termbits.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <array>
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

#include "Helpers/DescriptorWrapper.hpp"
#include "Version.hpp"

namespace MbInterface {

class MbSerial final {
public:
  struct TTYConfig final {
    std::string Port;
    size_t Baudrate;

    enum class StopbitsCount { One, Two };
    StopbitsCount Stopbits;

    bool ParityBit;
    float Timeout;
  };

  struct SOM {
    using Type = uint8_t;
    static constexpr Type SOM1 = 0xFF;
    static constexpr Type SOM2 = 0xAA;
    static constexpr Type SOM3 = 0xAF;
  };

  struct OutPackage {
    uint8_t ProcedureID;

    const uint8_t *Data;
    uint8_t Size;
  };

  struct InPackage {
    uint8_t Size;
    uint8_t Error;

    const uint8_t *Data;
  };

private:
  static constexpr size_t BufferSize = 512;
  std::array<uint8_t, BufferSize> Buffer;

  Helpers::DescriptorWrapper Fd;
  TTYConfig TTY;

  bool HasError = false;
  std::string Error;

private:
  bool MakeError(const std::string &msg);
  bool MakeTTYError(const TTYConfig &tty, const std::string &msg);
  void SerializePackageToBuf(const OutPackage &package, size_t *size);
  bool Read(uint8_t *buf, size_t size);
  bool Write(const uint8_t *buf, size_t size);
  bool ReadToBuf(size_t size);

public:
  MbSerial() = default;

  MbSerial(const MbSerial &) = delete;
  MbSerial &operator=(const MbSerial &) = delete;

  MbSerial(MbSerial &&) = default;
  MbSerial &operator=(MbSerial &&) = default;

  bool IsOk() const;
  std::string GetError() const;

  bool Configure(const TTYConfig &ttyConfig);

  bool Send(const OutPackage &package);
  bool Receive(InPackage &package);
};

} // namespace MbInterface
