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
  static constexpr uint8_t SOM3 = 0xFF;
};

struct Periphery {
  static constexpr uint8_t Body = 0;
  static constexpr uint8_t Imu = 0;
};

class OutPackage {
  uint8_t PeripheryId;
  uint8_t RequestSize;
  uint8_t ResponceSize;
  uint8_t MetaInfo;
  std::vector<uint8_t> Data;

public:
  OutPackage(uint8_t peripheryId, uint8_t responceSize, uint8_t metaInfo,
             std::vector<uint8_t> &&data)
      : PeripheryId{peripheryId}, RequestSize{data.size()},
        ResponceSize{responceSize}, MetaInfo{metaInfo}, Data{std::move(data)} {}

  size_t GetSize() const { return Data.size() + 7; }

  void SerializeTo(uint8_t *dest, size_t *size) const {
    assert(dest);
    assert(size);

    *size = GetSize();

    uint8_t *ptr = dest;

    *(ptr++) = SOMValues::SOM1;
    *(ptr++) = SOMValues::SOM2;
    *(ptr++) = PeripheryId;
    *(ptr++) = RequestSize;
    *(ptr++) = ResponceSize;
    *(ptr++) = MetaInfo;

    memcpy(ptr, Data.data(), Data.size());
    ptr += Data.size();

    *ptr = SOMValues::SOM3;
  }
};

struct InPackage {
  uint8_t PeripheryId;
  uint8_t MetaInfo;
  uint8_t Error;
  std::vector<uint8_t> Data;
};

class MBInterface final {
  bool HasError = false;
  std::string Error;

  DescriptorWrapper Fd;
  TTYConfig TTY;

  std::array<uint8_t, 512> Buffer;

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

public:
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

    assert(package.GetSize() <= 512);

    size_t size;
    package.SerializeTo(Buffer.data(), &size);

    if (write(Fd.Get(), Buffer.data(), size) <= 0)
      return MakeTTYError(TTY, "Failed to write");

    return true;
  }

  bool Read(size_t numBytes) {
    if (read(Fd.Get(), Buffer.data(), numBytes) <= 0)
      return MakeTTYError(TTY, "Failed to read");
    return true;
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

  bool Receive(size_t responceSize, InPackage &package) {
    if (read(Fd.Get(), Buffer.data(), 1) <= 0)
      return MakeTTYError(TTY, "Failed to read");

    if (!EnsureSom(256))
      return MakeError("Port " + TTY.Port + ": Sequence corrupted");

    if (!Read(1))
      return false;
    if (Buffer[0] != SOMValues::SOM2)
      return MakeTTYError(TTY, "Broken package");

    if (!Read(1))
      return false;
    package.PeripheryId = Buffer[0];

    if (!Read(1))
      return false;
    package.MetaInfo = Buffer[0];

    if (!Read(1))
      return false;
    package.Error = Buffer[0];

    if (!Read(responceSize))
      return false;
    
    package.Data.resize(responceSize);
    std::copy(Buffer.begin(), Buffer.begin() + responceSize,
              package.Data.begin());

    if (!Read(1)) return false;

    if (Buffer[0] != SOMValues::SOM3)
      return MakeTTYError(TTY, "Unexpected end of package");

    return true;
  }

  bool IsOk() const {
    return HasError;
  }

  std::string GetError() const {
    return Error;
  }
};

} // namespace Roki