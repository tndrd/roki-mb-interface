#include "SerialInterface.hpp"

namespace Roki
{

  bool SerialInterface::MakeError(const std::string &msg)
  {
    Error = msg;
    HasError = true;
    return false;
  }

  bool SerialInterface::MakeTTYError(const TTYConfig &tty, const std::string &msg)
  {
    return MakeError("Port " + tty.Port + ": " + msg + ": " +
                     std::string{strerror(errno)});
  }

  void SerialInterface::SerializePackageToBuf(const OutPackage &package,
                                          size_t *size)
  {
    assert(package.Data);
    assert(size);

    *size = package.RequestSize + 7;
    assert(*size < BufferSize);

    uint8_t *ptr = Buffer.data();

    *(ptr++) = SOM::SOM1;
    *(ptr++) = SOM::SOM2;
    *(ptr++) = package.PeripheryId;
    *(ptr++) = package.RequestSize;
    *(ptr++) = package.ResponceSize;
    *(ptr++) = package.MetaInfo;

    memcpy(ptr, package.Data, package.RequestSize);
    ptr += package.RequestSize;

    *ptr = SOM::SOM3;
  }

  bool SerialInterface::Read(uint8_t *buf, size_t size)
  {
    size_t total = 0;
    do
    {
      int ret = read(Fd.Get(), buf + total, size - total);
      if (ret < 0)
        return MakeTTYError(TTY, "Failed to read");
      else if (ret == 0)
        return MakeError("Port " + TTY.Port + ": Read timeout");
      total += ret;
    } while (total < size);
    return true;
  }

  bool SerialInterface::Write(const uint8_t *buf, size_t size)
  {
    size_t total = 0;
    do
    {
      int ret = write(Fd.Get(), buf + total, size - total);
      if (ret < 0)
        return MakeTTYError(TTY, "Failed to write");
      else if (ret == 0)
        return MakeError("Port " + TTY.Port + ": Write timeout");
      total += ret;

    } while (total < size);
    return true;
  }

  bool SerialInterface::ReadToBuf(size_t size) { return Read(Buffer.data(), size); }
  bool SerialInterface::IsOk() const { return !HasError; }
  std::string SerialInterface::GetError() const { return Error; }

  bool SerialInterface::Configure(const TTYConfig &ttyConfig)
  {

    Helpers::DescriptorWrapper newFd{open(ttyConfig.Port.c_str(), O_RDWR)};

    if (!newFd.IsValid())
      return MakeTTYError(ttyConfig, "Failed to open");

    termios2 tty;

    if (ioctl(newFd.Get(), TCGETS2, &tty))
      return MakeTTYError(ttyConfig, "Failed to get tty settings");

    tty.c_cflag &= ~CBAUD;
    tty.c_cflag |= CBAUDEX;
    tty.c_ispeed = ttyConfig.Baudrate;
    tty.c_ospeed = ttyConfig.Baudrate;

    if (ttyConfig.Stopbits == TTYConfig::StopbitsCount::One)
      tty.c_cflag &= ~CSTOPB;
    if (ttyConfig.Stopbits == TTYConfig::StopbitsCount::Two)
      tty.c_cflag |= CSTOPB;

    if (!ttyConfig.ParityBit)
      tty.c_cflag &= ~PARENB;
    if (ttyConfig.ParityBit)
      tty.c_cflag |= PARENB;

    if (ttyConfig.Timeout > 25) // Maximum posix timeout is 25.5 seconds
      return MakeError("Port " + ttyConfig.Port +
                       ": Timeout should be less than 25 seconds");

    uint8_t vtime = static_cast<uint8_t>(ttyConfig.Timeout * 10);

    tty.c_cc[VTIME] = vtime;
    tty.c_cc[VMIN] = 0;

    tty.c_lflag &= ~ICANON;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_lflag &= ~ECHO;                                                        // Disable echo
    tty.c_lflag &= ~ECHOE;                                                       // Disable erasure
    tty.c_lflag &= ~ECHONL;                                                      // Disable new-line echo
    tty.c_lflag &= ~ISIG;                                                        // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST;                                                       // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR;                                                       // Prevent conversion of newline to carriage return/line feed

    if (ioctl(newFd.Get(), TCSETS2, &tty))
      return MakeTTYError(ttyConfig, "Failed to set up");

    Fd = std::move(newFd);
    TTY = ttyConfig;
    return true;
  }

  bool SerialInterface::Send(const OutPackage &package)
  {
    if (!Fd.IsValid())
      return MakeError("TTY Interface is not set up");

    size_t size;
    SerializePackageToBuf(package, &size);

#ifdef DBGMB
    std::cout << "Sent: ";
    for (int i = 0; i < size; ++i)
    {
      std::cout << std::hex << +Buffer[i] << " ";
    }
    std::cout << std::dec << std::endl;
#endif

    if (!Write(Buffer.data(), size))
      return false;

    return true;
  }

  bool SerialInterface::Receive(size_t responceSize, InPackage &package)
  {

#ifdef DBGMB
    std::cout << "Reading " << responceSize + 3 + 3 << " bytes..." << std::endl;
    std::cout << "SOM1: " << std::flush;
#endif

    if (!ReadToBuf(1))
      return false;

    if (Buffer[0] != SOM::SOM1)
      return MakeError("Port " + TTY.Port + "Invalid SOM1 byte: " + std::to_string(+Buffer[0]));
    ;

#ifdef DBGMB
    std::cout << "OK" << std::endl;
    std::cout << "SOM2: " << std::flush;
#endif

    if (!ReadToBuf(1))
      return false;
    if (Buffer[0] != SOM::SOM2)
      return MakeTTYError(TTY, "Broken package");

#ifdef DBGMB
    std::cout << "OK" << std::endl;
    std::cout << "PeripheryId: " << std::flush;
#endif

    if (!ReadToBuf(1))
      return false;
    package.PeripheryId = Buffer[0];

#ifdef DBGMB
    std::cout << +package.PeripheryId << std::endl;
    std::cout << "MetaInfo: " << std::flush;
#endif

    if (!ReadToBuf(1))
      return false;
    package.MetaInfo = Buffer[0];

#ifdef DBGMB
    std::cout << +package.MetaInfo << std::endl;
    std::cout << "Error: " << std::flush;
#endif

    if (!ReadToBuf(1))
      return false;
    package.Error = Buffer[0];

#ifdef DBGMB
    std::cout << +package.Error << std::endl;
    std::cout << "Data: " << std::flush;
#endif

    if (!ReadToBuf(responceSize))
      return false;

    package.Data = Buffer.data();
    package.ResponceSize = responceSize;

#ifdef DBGMB
    for (int i = 0; i < responceSize; ++i)
    {
      std::cout << std::hex << +Buffer[i] << " ";
    }
    std::cout << std::dec << std::endl;

    std::cout << "SOM3: " << std::flush;
#endif

    uint8_t som;
    if (!Read(&som, 1))
      return false;

    if (som != SOM::SOM3)
      return MakeError("Port " + TTY.Port + "Unexpected end of package");

#ifdef DBGMB
    std::cout << "OK" << std::endl;
    std::cout << "Package OK\n" << std::endl;
#endif

    return true;
  }

} // namespace Roki