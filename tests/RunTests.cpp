#include "MbDefaultConfig.hpp"
#include "MbSerial.hpp"
#include "Motherboard.hpp"
#include "Rcb4BaseClass.hpp"
#include "RokiRcb4.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <gtest/gtest.h>

using namespace Roki;

#define MB_CALL(foo) ASSERT_TRUE(mb.foo) << mb.GetError() << std::endl
#define INIT_MB   \
  Motherboard mb; \
  ASSERT_TRUE(MbDefaultConfig(mb)) << mb.GetError() << std::endl

#define INIT_RCB     \
  INIT_MB;           \
  RokiRcb4 rcb4{mb}; \
  ASSERT_TRUE(rcb4.checkAcknowledge()) << rcb4.GetError() << std::endl;

#define RCB_CALL(foo) ASSERT_TRUE(rcb4.foo) << rcb4.GetError() << std::endl;

bool AskPrompt(const std::string &msg)
{
  std::cerr << msg << "? [y/n]" << std::endl;
  char resp = getchar();
  getchar();

  return resp == 'y';
}

#define PROMPT(msg) ASSERT_TRUE(AskPrompt(msg))

bool RokiPyTestRun(const std::string &name)
{
  pid_t pid = fork();

  if (pid == 0)
  { // child
    std::string path = "tests/" + name;
    execlp("python3", "python3", path.data(), NULL);

    perror("Failed to execute");
    exit(1);
  }
  if (pid > 0)
  { // parent
    int status;
    wait(&status);

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
      return true;
    else
      return false;
  }

  perror("Failed to fork");
  return false;
}

#define RPTEST(name) ASSERT_TRUE(RokiPyTestRun(name)) << "Failed to run test " << name << std::endl;

TEST(SerialInterface, NoFile)
{
  MbSerial si;
  MbSerial::TTYConfig tty;

  tty.Baudrate = 123456;
  tty.ParityBit = true;
  tty.Stopbits = MbSerial::TTYConfig::StopbitsCount::One;
  tty.Timeout = 12;
  tty.Port = "TRASH";

  ASSERT_FALSE(si.Configure(tty)) << si.GetError() << std::endl;
}

TEST(SerialInterface, WrongTimeout)
{
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

TEST(Motherboard, GetIMULatest)
{
  INIT_MB;

  IMUFrame frame;
  MB_CALL(GetIMULatest(frame));
}

TEST(Motherboard, ConfigureStrobeFilter)
{
  INIT_MB;

  MB_CALL(ConfigureStrobeFilter(0, 0));
}

TEST(Motherboard, GetStrobeDuration)
{
  RPTEST("TakePhotos.py");

  INIT_MB;

  uint8_t width;
  MB_CALL(GetStrobeWidth(width));

  ASSERT_EQ(width, 16);
}

TEST(Motherboard, SetIMUStrobeOffset)
{
  INIT_MB;

  MB_CALL(SetIMUStrobeOffset(0));
}

TEST(Motherboard, SetBodyStrobeOffset)
{
  INIT_MB;

  MB_CALL(SetBodyStrobeOffset(0));
}

TEST(Motherboard, GetIMUContainerInfo)
{
  INIT_MB;

  FrameContainerInfo info;

  MB_CALL(GetIMUContainerInfo(info));
}

TEST(Motherboard, GetIMUFrame)
{
  RPTEST("TakePhotos.py");

  INIT_MB;

  FrameContainerInfo info;
  IMUFrame frame;

  MB_CALL(GetIMUContainerInfo(info));

  ASSERT_GT(info.NumAv, 0) << "Queue empty" << std::endl;

  MB_CALL(GetIMUFrame(info.First, frame));

  ASSERT_FALSE(mb.GetIMUFrame(info.First - 1, frame))
      << "This frame should not be available" << std::endl;
}

TEST(Motherboard, GetBodyContainerInfo)
{
  INIT_MB;

  FrameContainerInfo dummy;

  MB_CALL(GetBodyContainerInfo(dummy));
}

TEST(Motherboard, GetBodyFrame)
{
  RPTEST("TakePhotos.py");

  INIT_MB;

  FrameContainerInfo info;
  BodyResponce responce;

  MB_CALL(GetBodyContainerInfo(info));

  ASSERT_GT(info.NumAv, 0) << "Queue empty" << std::endl;

  MB_CALL(GetBodyFrame(info.First, responce));

  ASSERT_FALSE(mb.GetBodyFrame(info.First - 1, responce))
      << "This frame should not be available" << std::endl;
}

TEST(Motherboard, ResetStrobeContatiners)
{
  RPTEST("TestResetSC.py");
}

TEST(Motherboard, BodySendForward)
{
  INIT_MB;

  Rcb4BaseClass rcb4;

  std::array<uint8_t, 4> requestData;
  std::array<uint8_t, 4> responceData;
  rcb4.acknowledgeCmd(requestData.data());

  MB_CALL(BodySendForward(requestData.data(), 4, responceData.data(), 4));

  ASSERT_EQ(requestData, responceData);
}

TEST(Motherboard, GetBodyQueueInfo)
{
  INIT_MB;

  BodyQueueInfo info;
  MB_CALL(GetBodyQueueInfo(info));
}

TEST(Motherboard, SetBodyQueuePeriod)
{
  INIT_MB;

  MB_CALL(SetBodyQueuePeriod(20));
  ASSERT_FALSE(mb.SetBodyQueuePeriod(0));
}

TEST(Motherboard, BodySendQueue)
{
  INIT_MB;
  Rcb4BaseClass rcb4;
  BodyQueueInfo info;

  std::array<uint8_t, 4> requestData;
  rcb4.acknowledgeCmd(requestData.data());

  MB_CALL(GetBodyQueueInfo(info));

  for (int i = 0; i < info.Capacity; ++i)
  {
    MB_CALL(BodySendQueue(requestData.data(), 4, 4));
  }

  int attempts = 0;
  while (mb.BodySendQueue(requestData.data(), 4, 4))
  {
    ASSERT_TRUE(attempts++ < info.Capacity)
        << "Failed to overflow body queue" << std::endl;
  }
}

TEST(Rcb4, Acknowledge)
{
  INIT_RCB;
}

TEST(Rcb4, SetServoPosSync)
{
  INIT_RCB;

  RokiRcb4::ServoData sd;

  sd.Id = 8;
  sd.Sio = 0;
  sd.Data = 9000;

  RCB_CALL(setServoPos(&sd, 1, 5));
  PROMPT("Servo has moved");

  sd.Data = 7500;
  RCB_CALL(setServoPos(&sd, 1, 5));
}

TEST(Rcb4, SetServoPosAsync)
{
  INIT_RCB;

  RokiRcb4::ServoData sd;

  sd.Id = 8;
  sd.Sio = 0;
  sd.Data = 9000;

  RCB_CALL(setServoPosAsync(&sd, 1, 5));
  PROMPT("Servo has moved");

  sd.Data = 7500;
  RCB_CALL(setServoPosAsync(&sd, 1, 5));
}

TEST(Motherboard, TestBodyQueue)
{
  RPTEST("TestBQ.py");
}

TEST(Rcb4, ReadRam)
{
  INIT_RCB;

  std::array<uint8_t, 8> zeroes = {};
  std::array<uint8_t, 8> rxData;

  RCB_CALL(moveRamToComCmdSynchronize(0x0060, 8, rxData.data()));

  ASSERT_NE(rxData, zeroes) << "read resulted in zeroes" << std::endl;
}

TEST(Motherboard, TestStrobeFilter)
{
  RPTEST("TestSF.py");
}

#undef MB_CALL
#undef INIT_MB
#undef INIT_RCB
#undef RCB_CALL
#undef PROMPT
#undef RPTEST