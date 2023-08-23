#pragma once

#include "IMURPC.hpp"

namespace Roki
{
    class IMUInterface : public IMURPCStub
    {
    public:
        bool GetFrame(MBInterface &mbi, uint16_t seq, IMUFrame &frame);
        bool GetInfo(MBInterface &mbi, IMUInfo &info);
        bool GetLastFrame(MBInterface &mbi, IMUFrame &frame);
        bool ResetCounter(MBInterface &mbi);
    };
}