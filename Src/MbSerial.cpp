#include "MbSerial.hpp"

namespace Roki {

bool MbSerial::MakeError(const std::string &msg) {
  Error = msg;
  HasError = true;
  return false;
}

bool MbSerial::MakeTTYError(const TTYConfig &tty, const std::string &msg) {
  return MakeError("Port " + tty.Port + ": " + msg + ": " +
                   std::string{strerror(errno)});
}

void MbSerial::SerializePackageToBuf(const OutPackage &package, size_t *size) {
  assert(package.Data);
  assert(size);

  *size = package.Size + 5;
  assert(*size < BufferSize);

  uint8_t *ptr = Buffer.data();

  *(ptr++) = SOM::SOM1;
  *(ptr++) = SOM::SOM2;
  *(ptr++) = package.ProcedureID;
  *(ptr++) = package.Size;

  memcpy(ptr, package.Data, package.Size);
  ptr += package.Size;

  *ptr = SOM::SOM3;
}

bool MbSerial::Read(uint8_t *buf, size_t size) {
  size_t total = 0;
  while (total < size) {
    int ret = read(Fd.Get(), buf + total, size - total);
    if (ret < 0)
      return MakeTTYError(TTY, "Failed to read");
    else if (ret == 0)
      return MakeError("Port " + TTY.Port + ": Read timeout");
    total += ret;
  }
  return true;
}

bool MbSerial::Write(const uint8_t *buf, size_t size) {
  size_t total = 0;
  while (total < size) {
    int ret = write(Fd.Get(), buf + total, size - total);
    if (ret < 0)
      return MakeTTYError(TTY, "Failed to write");
    else if (ret == 0)
      return MakeError("Port " + TTY.Port + ": Write timeout");
    total += ret;
  }
  return true;
}

bool MbSerial::ReadToBuf(size_t size) { return Read(Buffer.data(), size); }
bool MbSerial::IsOk() const { return !HasError; }
std::string MbSerial::GetError() const { return Error; }

bool MbSerial::Configure(const TTYConfig &ttyConfig) {

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
  tty.c_lflag &= ~ECHO;   // Disable echo
  tty.c_lflag &= ~ECHOE;  // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG;   // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR |
                   ICRNL); // Disable any special handling of received bytes
  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g.
                         // newline chars)
  tty.c_oflag &=
      ~ONLCR; // Prevent conversion of newline to carriage return/line feed

  if (ioctl(newFd.Get(), TCSETS2, &tty))
    return MakeTTYError(ttyConfig, "Failed to set up");

  Fd = std::move(newFd);
  TTY = ttyConfig;
  return true;
}

bool MbSerial::Send(const OutPackage &package) {
  if (!Fd.IsValid())
    return MakeError("TTY Interface is not set up");

  size_t size;
  SerializePackageToBuf(package, &size);

#ifdef DBGMB
  std::cout << "Sent: ";
  for (int i = 0; i < size; ++i) {
    std::cout << std::hex << +Buffer[i] << " ";
  }
  std::cout << std::dec << std::endl;

  std::cout << std::dec << "Data [" << +package.Size << "]: " << std::flush;
  for (int i = 0; i < package.Size; ++i) {
    std::cout << std::hex << +package.Data[i] << " ";
  }
  std::cout << std::dec << std::endl;
#endif

  if (!Write(Buffer.data(), size))
    return false;

  return true;
}

bool MbSerial::Receive(InPackage &package) {

#ifdef DBGMB
  std::cout << "Reading package..." << std::endl;
  std::cout << "SOM1: " << std::flush;
#endif

  if (!ReadToBuf(1))
    return false;

  if (Buffer[0] != SOM::SOM1)
    return MakeError("Port " + TTY.Port +
                     "Invalid SOM1 byte: " + std::to_string(+Buffer[0]));

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
  std::cout << "Error: " << std::flush;
#endif

  if (!ReadToBuf(1))
    return false;
  package.Error = Buffer[0];

#ifdef DBGMB
  std::cout << +package.Error << std::endl;
  std::cout << "Size: " << std::flush;
#endif

  if (!ReadToBuf(1))
    return false;

  package.Size = Buffer[0];

#ifdef DBGMB
  std::cout << +package.Size << std::endl;
  std::cout << "Data: " << std::flush;
#endif
  if (!ReadToBuf(package.Size))
    return false;

  package.Data = Buffer.data();

#ifdef DBGMB
  for (int i = 0; i < package.Size; ++i) {
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