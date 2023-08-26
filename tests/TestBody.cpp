#include "MBInterface.hpp"
#include "BodyInterface.hpp"
#include "BodySingleSender.hpp"

#include <chrono>
#include <thread>

int main()
{
  Roki::MBInterface mbi;
  Roki::MBInterface::TTYConfig tty;

  tty.Port = "/dev/ttyAMA2";
  tty.Baudrate = 9600;
  tty.ParityBit = true;
  tty.Stopbits = tty.STOPBITS_ONE;
  tty.Timeout = 25;

  if (!mbi.Configure(tty))
  {
    std::cout << mbi.GetError() << std::endl;
    exit(1);
  }

  Roki::BodyInterface body;
  Roki::BodySingleSender sender;

  std::vector<uint8_t> data {1, 2, 3, 4, 5};
  std::array<uint8_t, 64> buf;

  if(!sender.Send(mbi, body, data.data(), buf.data(), data.size(), 12)) {
    std::cout << body.GetError() << std::endl;
    exit(1);
  }
}
