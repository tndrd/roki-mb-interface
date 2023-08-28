#pragma once

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <asm/termbits.h>
#include <unistd.h>

#include <array>
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

#include "Helpers.hpp"

namespace Roki {

class SerialInterface final {
public:
  struct TTYConfig final {
    std::string Port;
    size_t Baudrate;

    enum { STOPBITS_ONE, STOPBITS_TWO } Stopbits;

    bool ParityBit;
    float Timeout;
  };

  struct SOM {
    using Type = uint8_t;
    static constexpr Type SOM1 = 0xFF;
    static constexpr Type SOM2 = 0xAA;
    static constexpr Type SOM3 = 0xAF;
  };

  struct Periphery {
    using Type = uint8_t;
    static constexpr Type Body = 0;
    static constexpr Type Imu = 1;
  };

  struct OutPackage {
    Periphery::Type PeripheryId;
    uint8_t ResponceSize;
    uint8_t MetaInfo;

    const uint8_t *Data;
    uint8_t RequestSize;
  };

  struct InPackage {
    uint8_t PeripheryId;
    uint8_t MetaInfo;
    uint8_t Error;

    const uint8_t *Data;
    uint8_t ResponceSize;
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
  bool EnsureSOM1(size_t windowSize);

public:
  SerialInterface() = default;

  SerialInterface(const SerialInterface &) = delete;
  SerialInterface &operator=(const SerialInterface &) = delete;

  SerialInterface(SerialInterface &&) = default;
  SerialInterface &operator=(SerialInterface &&) = default;

  bool IsOk() const;
  std::string GetError() const;

  bool Configure(const TTYConfig &ttyConfig);
  bool Send(const OutPackage &package);
  bool Receive(size_t responceSize, InPackage &package);
};

} // namespace Roki
