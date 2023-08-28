#include "SystemState.hpp"

namespace Roki
{

    SystemState SystemState::DeserializeFrom(uint8_t const **ptr)
    {
        SystemState ss;

        ss.Body = BodyRPC::Info::DeserializeFrom(ptr);
        ss.Imu = IMURPC::IMUInfo::DeserializeFrom(ptr);
        ss.Orientation = IMURPC::IMUFrame::DeserializeFrom(ptr);

        return ss;
    }
    bool SystemStateListener::MakeError(const std::string &msg)
    {
        HasError = true;
        Error = msg;
        return false;
    }

    void SystemStateListener::ReadLoop(SerialInterface &stream)
    {
        while (Active)
        {
            SerialInterface::InPackage package;
            if (!stream.Receive(SystemState::Size, package)) {
                MakeError(stream.GetError());
                ReceivedOne = true;
                return;
            }
            const uint8_t *ptr = package.Data;
            state = SystemState::DeserializeFrom(&ptr);
            ReceivedOne = true;

        }
    }

    bool SystemStateListener::GetState(SystemState &result)
    {
        if (!Active)
            return MakeError("Not started yet");

        while(!ReceivedOne) {}

        if (HasError)
            return false;

        result = state;

        return true;
    }

    bool SystemStateListener::Start(SerialInterface &stream)
    {
        if (Active)
            return true;

        Active = true;
        ReceivedOne = false;
        ReadThread = std::thread{&SystemStateListener::ReadLoop, this, std::ref(stream)};

        return true;
    }

    bool SystemStateListener::Stop()
    {
        if (!Active)
            return true;

        Active = false;
        ReadThread.join();

        return true;
    }

    bool SystemStateListener::IsActive() const { return Active; }

    bool SystemStateListener::IsOk() const
    {
        return !HasError;
    }
    std::string SystemStateListener::GetError() const
    {
        return Error;
    }
}