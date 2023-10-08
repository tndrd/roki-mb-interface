#pragma once

#include "MbMessages.hpp"
#include "MbSerial.hpp"
#include "Helpers/IMUFrame.hpp"

namespace Roki {
    using IMUFrame = Helpers::IMUFrame;
    using Quaternion = IMUFrame::Quaternion;
    using Timestamp =  IMUFrame::TimestampT;
    using FrameContainerInfo = Messages::FrameContainerInfo;
    using TTYConfig = MbSerial::TTYConfig;
    using BodyQueueInfo = Messages::BodyQueueInfo;
    using BodyResponce = Messages::BodyResponce;
    using Version = Messages::Version;
}