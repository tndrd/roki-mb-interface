#include "MbDefaultConfig.hpp"
#include "MbSerial.hpp"
#include "Motherboard.hpp"
#include "Rcb4BaseClass.hpp"
#include <gtest/gtest.h>

using namespace Roki;

#define MB_CALL(foo) ASSERT_TRUE(mb.foo) << mb.GetError() << std::endl
#define INIT_MB                                                                \
  Motherboard mb;                                                              \
  ASSERT_TRUE(MbDefaultConfig(mb)) << mb.GetError() << std::endl

TEST(SerialInterface, NoFile) {
  MbSerial si;
  MbSerial::TTYConfig tty;

  tty.Baudrate = 123456;
  tty.ParityBit = true;
  tty.Stopbits = MbSerial::TTYConfig::StopbitsCount::One;
  tty.Timeout = 12;
  tty.Port = "TRASH";

  ASSERT_FALSE(si.Configure(tty)) << si.GetError() << std::endl;
}

TEST(SerialInterface, WrongTimeout) {
  MbSerial si;
  MbSerial::TTYConfig tty;

  tty.Baudrate = 123456;
  tty.ParityBit = true;
  tty.Stopbits = MbSerial::TTYConfig::StopbitsCount::One;
  tty.Timeout = 42;
  tty.Port = "/dev/ttyAMA2";

  ASSERT_FALSE(si.Configure(tty));
}

TEST(Motherboard, DefaultConfigure) { INIT_MB; }

TEST(Motherboard, GetIMULatest) {
  INIT_MB;

  IMUFrame frame;
  MB_CALL(GetIMULatest(frame));
}

TEST(Motherboard, ConfigureStrobeFilter) {
  INIT_MB;

  MB_CALL(ConfigureStrobeFilter(0, 0));
}

TEST(Motherboard, GetStrobeDuration) {
  INIT_MB;

  uint8_t dummy;

  MB_CALL(GetStrobeWidth(dummy));
}

TEST(Motherboard, SetIMUStrobeOffset) {
  INIT_MB;

  MB_CALL(SetIMUStrobeOffset(0));
}

TEST(Motherboard, SetBodyStrobeOffset) {
  INIT_MB;

  MB_CALL(SetBodyStrobeOffset(0));
}

TEST(Motherboard, ResetStrobeContatiners) {
  INIT_MB;

  MB_CALL(ResetStrobeContainers());
}

TEST(Motherboard, GetIMUContainerInfo) {
  INIT_MB;

  FrameContainerInfo dummy;

  MB_CALL(GetIMUContainerInfo(dummy));
}

TEST(Motherboard, GetIMUFrame) {
  INIT_MB;

  FrameContainerInfo info;
  IMUFrame frame;

  MB_CALL(GetIMUContainerInfo(info));

  ASSERT_GT(info.NumAv, 0) << "Please take some photos to test it" << std::endl;

  MB_CALL(GetIMUFrame(info.First, frame));

  ASSERT_FALSE(mb.GetIMUFrame(info.First - 1, frame))
      << "This frame should not be available" << std::endl;
}

TEST(Motherboard, GetBodyContainerInfo) {
  INIT_MB;

  FrameContainerInfo dummy;

  MB_CALL(GetBodyContainerInfo(dummy));
}

TEST(Motherboard, GetBodyFrame) {
  INIT_MB;

  FrameContainerInfo info;
  BodyResponce responce;

  MB_CALL(GetBodyContainerInfo(info));

  ASSERT_GT(info.NumAv, 0) << "Please take some photos to test it" << std::endl;

  MB_CALL(GetBodyFrame(info.First, responce));

  ASSERT_FALSE(mb.GetBodyFrame(info.First - 1, responce))
      << "This frame should not be available" << std::endl;
}

TEST(Motherboard, BodySendForward) {
  INIT_MB;

  Rcb4BaseClass rcb4;

  std::array<uint8_t, 4> requestData;
  std::array<uint8_t, 4> responceData;
  rcb4.acknowledgeCmd(requestData.data());

  MB_CALL(BodySendForward(requestData.data(), 4, responceData.data(), 4));

  ASSERT_EQ(requestData, responceData);
}

TEST(Motherboard, GetBodyQueueInfo) {
  INIT_MB;

  BodyQueueInfo info;
  MB_CALL(GetBodyQueueInfo(info));
}

TEST(Motherboard, SetBodyQueuePeriod) {
  INIT_MB;

  MB_CALL(SetBodyQueuePeriod(20));
  ASSERT_FALSE(mb.SetBodyQueuePeriod(0));
}

TEST(Motherboard, BodySendQueue) {
  INIT_MB;
  Rcb4BaseClass rcb4;
  BodyQueueInfo info;

  std::array<uint8_t, 4> requestData;
  rcb4.acknowledgeCmd(requestData.data());

  MB_CALL(GetBodyQueueInfo(info));

  for (int i = 0; i < info.Capacity; ++i) {
    MB_CALL(BodySendQueue(requestData.data(), 4, 4));
  }

  int attempts = 0;
  while (mb.BodySendQueue(requestData.data(), 4, 4)) {
    ASSERT_TRUE(attempts++ < info.Capacity)
        << "Failed to overflow body queue" << std::endl;
  }
}