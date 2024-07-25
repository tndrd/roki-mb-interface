#include "roki-mb-interface/Motherboard.hpp"

namespace MbInterface {

bool Motherboard::MakeError(const std::string &msg) {
  HasError = true;
  Error = msg;
  return false;
}

bool Motherboard::MakeFooError(const std::string &fooName,
                               const std::string &msg) {
  return MakeError(fooName + ": " + msg);
}

std::string VersionToString(Version version) {
  return "v" + std::to_string(version.Major) + "." +
         std::to_string(version.Minor);
}

std::string LibraryVersionToString() {
  Version version;
  version.Major = INTERFACE_VERSION_MAJOR;
  version.Minor = INTERFACE_VERSION_MINOR;

  return VersionToString(version);
}

bool CheckVersion(Version version) {
  if (version.Major == INTERFACE_VERSION_MAJOR &&
      version.Minor == INTERFACE_VERSION_MINOR)
    return true;
  return false;
}

#define FOO_ERROR(msg) MakeFooError(__func__, msg)
#define CHECK_CLIENT_ERROR ok ? true : FOO_ERROR(Client.GetError())
#define CREATE_GUARD std::lock_guard<std::mutex> _(Mutex)

bool Motherboard::Configure(const TTYConfig &config) {
  CREATE_GUARD;
  Version version;

  if (!Serial.Configure(config))
    return FOO_ERROR(Serial.GetError());

  if (!GetVersion_NoLock(version))
    return FOO_ERROR(GetError());

  if (!CheckVersion(version))
    return FOO_ERROR("Version conflict: library " + LibraryVersionToString() +
                     " firmware " + VersionToString(version));

  return true;
}

bool Motherboard::GetIMUFrame(uint16_t seq, IMUFrame &result) {
  CREATE_GUARD;

  Messages::FrameNumber request;
  Messages::IMUFrameMsg responce;
  request.Seq = seq;

  bool ok = Client.PerformRPC<Proc::GetIMUFrame>(Serial, request, responce);

  if (ok)
    result = IMUFrame::ConvertFrom(responce);
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::GetBodyFrame(uint16_t seq, BodyResponce &result) {
  CREATE_GUARD;

  Messages::FrameNumber request;
  request.Seq = seq;

  bool ok = Client.PerformRPC<Proc::GetBodyFrame>(Serial, request, result);
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::GetIMUContainerInfo(FrameContainerInfo &result) {
  CREATE_GUARD;

  bool ok = Client.PerformRPC<Proc::GetIMUContainerInfo>(Serial, {}, result);
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::GetBodyContainerInfo(FrameContainerInfo &result) {
  CREATE_GUARD;

  bool ok = Client.PerformRPC<Proc::GetBodyContainerInfo>(Serial, {}, result);
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::ResetStrobeContainers() {
  CREATE_GUARD;

  Messages::Empty responce;
  bool ok =
      Client.PerformRPC<Proc::ResetStrobeContainers>(Serial, {}, responce);
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::SetIMUStrobeOffset(uint8_t offset) {
  CREATE_GUARD;

  Messages::Byte request;
  Messages::Empty responce;

  request.Value = offset;

  bool ok =
      Client.PerformRPC<Proc::SetIMUStrobeOffset>(Serial, request, responce);
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::SetBodyStrobeOffset(uint8_t offset) {
  CREATE_GUARD;

  Messages::Byte request;
  Messages::Empty responce;

  request.Value = offset;

  bool ok =
      Client.PerformRPC<Proc::SetBodyStrobeOffset>(Serial, request, responce);
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::GetIMULatest(IMUFrame &result) {
  CREATE_GUARD;

  Messages::IMUFrameMsg responce;

  bool ok = Client.PerformRPC<Proc::GetIMULatest>(Serial, {}, responce);
  if (ok)
    result = IMUFrame::ConvertFrom(responce);
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::GetStrobeWidth(uint8_t &result) {
  CREATE_GUARD;

  Messages::Byte responce;
  bool ok = Client.PerformRPC<Proc::GetStrobeWidth>(Serial, {}, responce);
  if (ok)
    result = responce.Value;
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::ConfigureStrobeFilter(uint8_t targetDuration,
                                        uint8_t durationThreshold) {
  CREATE_GUARD;

  Messages::StrobeFilterConfig request;
  Messages::Empty responce;
  request.TargetDuration = targetDuration;
  request.DurationThreshold = durationThreshold;

  bool ok =
      Client.PerformRPC<Proc::ConfigureStrobeFilter>(Serial, request, responce);
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::BodySendForward(const uint8_t *requestData,
                                  uint8_t requestSize, uint8_t *responceData,
                                  uint8_t responceSize) {
  CREATE_GUARD;

  Messages::BodyRequest request;
  Messages::BodyResponce responce;
  request.Data = requestData;
  request.RequestSize = requestSize;
  request.ResponceSize = responceSize;

  bool ok = Client.PerformRPC<Proc::BodySendForward>(Serial, request, responce);

  if (ok) {
    assert(responceSize == responce.ResponceSize);
    memcpy(responceData, responce.Data, responce.ResponceSize);
  }

  return CHECK_CLIENT_ERROR;
}

bool Motherboard::BodySendQueue(const uint8_t *requestData, uint8_t requestSize,
                                uint8_t responceSize, uint8_t pause) {
  CREATE_GUARD;

  Messages::BodyRequest request;
  Messages::Empty responce;
  request.Data = requestData;
  request.RequestSize = requestSize;
  request.ResponceSize = responceSize;

  Messages::BodyQueueRequest msg;
  msg.Request = request;
  msg.Pause = pause;

  bool ok = Client.PerformRPC<Proc::BodySendQueue>(Serial, msg, responce);
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::GetBodyQueueInfo(BodyQueueInfo &result) {
  CREATE_GUARD;

  bool ok = Client.PerformRPC<Proc::GetBodyQueueInfo>(Serial, {}, result);
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::SetBodyQueuePeriod(uint8_t periodMs) {
  CREATE_GUARD;

  Messages::PeriodMs request;
  Messages::Empty responce;
  request.Ms = periodMs;

  bool ok =
      Client.PerformRPC<Proc::SetBodyQueuePeriod>(Serial, request, responce);
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::GetVersion_NoLock(Version &result) {
  bool ok = Client.PerformRPC<Proc::GetVersion>(Serial, {}, result);
  return CHECK_CLIENT_ERROR;
}

bool Motherboard::GetVersion(Version &result) {
  CREATE_GUARD;
  return GetVersion_NoLock(result);
}

bool Motherboard::ResetBodyQueue() {
  CREATE_GUARD;

  Messages::Empty responce;
  bool ok = Client.PerformRPC<Proc::ResetBodyQueue>(Serial, {}, responce);
  return CHECK_CLIENT_ERROR;
}

#undef CHECK_CLIENT_ERROR
#undef FOO_ERROR
#undef CREATE_GUARD

bool Motherboard::IsOk() const { return HasError; }
std::string Motherboard::GetError() const { return Error; }

} // namespace MbInterface