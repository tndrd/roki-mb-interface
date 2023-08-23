#include "IMUInterface.hpp"
namespace Roki
{
    bool IMUInterface::GetFrame(MBInterface &mbi, uint16_t seq, IMUFrame &frame)
    {
        IMUFrameRequest request;
        request.seq = seq;
        return PerformRPC(mbi, request, frame);
    }

    bool IMUInterface::GetInfo(MBInterface &mbi, IMUInfo &info) {
        IMUInfoRequest request = {};
        return PerformRPC(mbi, request, info);
    }

    bool IMUInterface::GetLastFrame(MBInterface &mbi, IMUFrame &frame)
    {
        IMULatestRequest request = {};
        return PerformRPC(mbi, request, frame);
    }

    bool IMUInterface::ResetCounter(MBInterface &mbi)
    {
        IMUResetRequest request = {};
        Empty responce = {};
        return PerformRPC(mbi, request, responce);
    }
}