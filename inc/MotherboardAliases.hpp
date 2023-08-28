#pragma once

#include "Motherboard.hpp"

namespace Roki {
    using Quaternion = IMURPC::Quaternion;
    using Timestamp = IMURPC::TimestampT;
    using IMUFrame = IMURPC::IMUFrame;
    using IMUInfo = IMURPC::IMUInfo;
    using TTYConfig = SerialInterface::TTYConfig;
    using QueueInfo = BodyRPC::Info;
}