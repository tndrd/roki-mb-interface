#pragma once

#include "IMURPC.hpp"
#include "BodyRPC.hpp"
#include <thread>

namespace Roki
{

    struct SystemState
    {
        IMURPC::IMUInfo Imu;
        BodyRPC::Info Body;
        IMURPC::IMUFrame Orientation;

        static constexpr uint8_t Size = IMURPC::IMUInfo::Size + BodyRPC::Info::Size + IMURPC::IMUFrame::Size;

        static SystemState DeserializeFrom(uint8_t const **ptr);
    };

    class SystemStateListener
    {
    private:
        SystemState state;
        bool HasError = false;
        std::string Error;
        bool Active = false;
        bool ReceivedOne = false;

        std::thread ReadThread;

    private:
        bool MakeError(const std::string &msg);
        void StopReadLoopOnError(const std::string &msg);
        void ReadLoop(SerialInterface &stream);

    public:
        SystemStateListener() = default;

        SystemStateListener(const SystemStateListener &) = delete;
        SystemStateListener &operator=(const SystemStateListener &) = delete;

        SystemStateListener(SystemStateListener &&) = default;
        SystemStateListener &operator=(SystemStateListener &&) = default;

        bool GetState(SystemState &result);

        bool Start(SerialInterface &stream);
        bool Stop();
        bool IsActive() const;

        bool IsOk() const;
        std::string GetError() const;
    };

}