#pragma once

#include "MbMessages.hpp"
#include "MbSerial.hpp"

namespace Roki {
    using Quaternion = Messages::IMUFrame::Quaternion;
    using Timestamp =  Messages::IMUFrame::TimestampT;
    using IMUFrame =  Messages::IMUFrame;
    using FrameContainerInfo = Messages::FrameContainerInfo;
    using TTYConfig = MbSerial::TTYConfig;
    using BodyQueueInfo = Messages::BodyQueueInfo;
    using BodyResponce = Messages::BodyResponce;
    using Version = Messages::Version;
}