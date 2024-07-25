#pragma once

#include "roki-mb-service/MbMessages.hpp"
#include "roki-mb-interface/MbSerial.hpp"
#include "roki-mb-interface/Helpers/IMUFrame.hpp"

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