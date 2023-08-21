#include <errno.h> // Error integer and strerror() function
#include <fcntl.h> // Contains file controls like O_RDWR
#include <string.h>
#include <termios.h>
#include <unistd.h> // write(), read(), close()

#include <array>
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

namespace Roki {

struct DescriptorWrapper final {
private:
  int Fd = -1;

public:
  explicit DescriptorWrapper(int newFd) : Fd{newFd} {}
  bool IsValid() const { return Fd >= 0; }

  int Get() const { return Fd; }

  DescriptorWrapper(const DescriptorWrapper &) = delete;
  DescriptorWrapper &operator=(const DescriptorWrapper &) = delete;

  DescriptorWrapper(DescriptorWrapper &&rhs) : Fd{rhs.Fd} { rhs.Fd = -1; }
  DescriptorWrapper &operator=(DescriptorWrapper &&rhs) {
    std::swap(Fd, rhs.Fd);
    return *this;
  }

  ~DescriptorWrapper() {
    if (IsValid())
      close(Fd);
  }
};

class MBInterface final {
public:
  struct TTYConfig final {
    std::string Port;
    size_t Baudrate;

    enum { STOPBITS_ONE, STOPBITS_TWO } Stopbits;

    bool ParityBit;
    float Timeout;
  };

  struct SOMValues {
    static constexpr uint8_t SOM1 = 0xFF;
    static constexpr uint8_t SOM2 = 0xAA;
    static constexpr uint8_t SOM3 = 0xAF;
  };

  struct Periphery {
    static constexpr uint8_t Body = 0;
    static constexpr uint8_t Imu = 0;
  };

  struct OutPackage {
    uint8_t PeripheryId;
    uint8_t ResponceSize;
    uint8_t MetaInfo;

    const uint8_t *Data;
    uint8_t RequestSize;
  };

  struct InPackage {
    uint8_t PeripheryId;
    uint8_t MetaInfo;
    uint8_t Error;

    const uint8_t* Data;
    uint8_t ResponceSize;
  };

private:
  static constexpr uint8_t BufferSize = 512;

  DescriptorWrapper Fd;
  TTYConfig TTY;

  bool HasError = false;
  std::string Error;

  std::array<uint8_t, BufferSize> Buffer;

private:
  bool MakeError(const std::string &msg) {
    Error = msg;
    HasError = true;
    return false;
  }

  bool MakeTTYError(const TTYConfig &tty, const std::string &msg) {
    return MakeError("Port " + tty.Port + ": " + msg + ": " +
                     std::string{strerror(errno)});
  }

  void SerializePackageToBuf(const OutPackage &package, size_t *size) {
    assert(package.Data);
    assert(size);

    *size = package.RequestSize + 7;
    assert(*size < BufferSize);

    uint8_t *ptr = Buffer.data();

    *(ptr++) = SOMValues::SOM1;
    *(ptr++) = SOMValues::SOM2;
    *(ptr++) = package.PeripheryId;
    *(ptr++) = package.RequestSize;
    *(ptr++) = package.ResponceSize;
    *(ptr++) = package.MetaInfo;

    memcpy(ptr, package.Data, package.ResponceSize);
    ptr += package.ResponceSize;

    *ptr = SOMValues::SOM3;
  }

  bool Read(uint8_t* buf, size_t size) {
    if (read(Fd.Get(), buf, size) <= 0)
      return MakeTTYError(TTY, "Failed to read");
    return true;
  }

  bool ReadToBuf(size_t size) {
    return Read(Buffer.data(), size);
  }

  bool EnsureSom(size_t windowSize) {
    if (Buffer[0] == SOMValues::SOM1)
      return true;

    std::cout << "Warning: Package sequence corrupted, attempting to fix..."
              << std::endl;

    size_t i = 0;
    while (Buffer[0] != SOMValues::SOM1 && i++ < windowSize) {
      if (read(Fd.Get(), Buffer.data(), 1) <= 0)
        return false;
    }

    return Buffer[0] == SOMValues::SOM1;
  }

public:
  bool IsOk() const { return HasError; }
  std::string GetError() const { return Error; }

  MBInterface() = default;

  bool Configure(const TTYConfig &ttyConfig) {

    DescriptorWrapper newFd{open(ttyConfig.Port.c_str(), O_RDWR)};

    if (!newFd.IsValid())
      return MakeTTYError(ttyConfig, "Failed to open");

    termios tty;

    if (tcgetattr(newFd.Get(), &tty))
      return MakeTTYError(ttyConfig, "Failed to get tty settings");

    if (cfsetspeed(&tty, ttyConfig.Baudrate))
      return MakeTTYError(ttyConfig, "Failed to set baudrate");

    if (ttyConfig.Stopbits == TTYConfig::STOPBITS_ONE)
      tty.c_cflag &= ~CSTOPB;
    if (ttyConfig.Stopbits == TTYConfig::STOPBITS_TWO)
      tty.c_cflag |= CSTOPB;

    if (!ttyConfig.ParityBit)
      tty.c_cflag &= ~PARENB;
    if (ttyConfig.ParityBit)
      tty.c_cflag |= PARENB;

    if (ttyConfig.Timeout > 25) // Maximum posix timeout is 25.5 seconds
      return MakeError("Port " + ttyConfig.Port +
                       ": Timeout should be less than 25 seconds");

    tty.c_cc[VTIME] = static_cast<uint8_t>(ttyConfig.Timeout * 10);
    tty.c_cc[VMIN] = 0;

    if (tcsetattr(newFd.Get(), TCSANOW, &tty))
      return MakeTTYError(ttyConfig, "Failed to set up");

    Fd = std::move(newFd);
    TTY = ttyConfig;
    return true;
  }

  bool Send(const OutPackage &package) {
    if (!Fd.IsValid())
      return MakeError("TTY Interface is not set up");

    size_t size;
    SerializePackageToBuf(package, &size);

    if (write(Fd.Get(), Buffer.data(), size) <= 0)
      return MakeTTYError(TTY, "Failed to write");

    return true;
  }

  bool Receive(size_t responceSize, InPackage &package) {
    if (!ReadToBuf(1))
      return false;
    
    if (!EnsureSom(256))
      return MakeError("Port " + TTY.Port + ": Sequence corrupted");

    if (!ReadToBuf(1))
      return false;
    if (Buffer[0] != SOMValues::SOM2)
      return MakeTTYError(TTY, "Broken package");

    if (!ReadToBuf(1))
      return false;
    package.PeripheryId = Buffer[0];

    if (!ReadToBuf(1))
      return false;
    package.MetaInfo = Buffer[0];

    if (!ReadToBuf(1))
      return false;
    package.Error = Buffer[0];

    if (!ReadToBuf(responceSize))
      return false;

    package.Data = Buffer.data();

    uint8_t som;
    if (!Read(&som, 1))
      return false;

    if (som != SOMValues::SOM3)
      return MakeTTYError(TTY, "Unexpected end of package");

    return true;
  }
};

} // namespace Roki