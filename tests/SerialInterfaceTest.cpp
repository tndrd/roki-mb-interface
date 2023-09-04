#include <gtest/gtest.h>
#include "SerialInterface.hpp"

using namespace Roki;

TEST(SerialInterface, NoFile) {
    SerialInterface si;
    SerialInterface::TTYConfig tty;
    
    tty.Baudrate = 123456;
    tty.ParityBit = true;
    tty.Stopbits = SerialInterface::TTYConfig::StopbitsCount::One;
    tty.Timeout = 12;
    tty.Port = "TRASH";

    ASSERT_FALSE(si.Configure(tty));
}

TEST(SerialInterface, WrongTimeout) {
    SerialInterface si;
    SerialInterface::TTYConfig tty;
    
    tty.Baudrate = 123456;
    tty.ParityBit = true;
    tty.Stopbits = SerialInterface::TTYConfig::StopbitsCount::One;
    tty.Timeout = 42;
    tty.Port = "/dev/ttyAMA2";

    ASSERT_FALSE(si.Configure(tty));
}

TEST(SerialInterface, Acknowlege)