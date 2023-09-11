#include "IMURPC.hpp"
namespace Roki
{

  void IMURPC::SerializeToBuf(IMUFrameRequest request)
  {
    *reinterpret_cast<uint16_t *>(RequestBuffer.data()) = request.seq;
  }

  void IMURPC::SerializeToBuf(IMUInfoRequest request)
  {
    RequestBuffer[0] = 0;
  }

  void IMURPC::SerializeToBuf(IMULatestRequest request)
  {
    RequestBuffer[0] = 0;
  }

  void IMURPC::SerializeToBuf(IMUResetRequest request)
  {
    RequestBuffer[0] = 0;
  }

  void IMURPC::SerializeToBuf(SetStrobeOffsetRequest request)
  {
    RequestBuffer[0] = request.offset;
  }

  void IMURPC::SerializeToBuf(StrobeWidthRequest request)
  {
    RequestBuffer[0] = 0;
  }

  void IMURPC::SerializeToBuf(ConfigureFilterRequest request)
  {
    RequestBuffer[0] = request.TargetDuration;
    RequestBuffer[1] = request.DurationThreshold;
  }

  template <typename T>
  SerialInterface::OutPackage IMURPC::CreatePackage(T Request)
  {
    SerializeToBuf(Request);
    SerialInterface::OutPackage outPackage{SerialInterface::Periphery::Imu, T::ResponceType::Size,
                                           RequestMode::Serialize(T::Mode), RequestBuffer.data(), T::Size};
    return outPackage;
  }

  template <>
  IMURPC::IMUFrame
  IMURPC::DeserializeResponce(const SerialInterface::InPackage &package)
  {
    assert(package.ResponceSize == IMUFrame::Size);

    const uint8_t *ptr = package.Data;

    return IMUFrame::DeserializeFrom(&ptr);
  }

  template <>
  IMURPC::IMUInfo
  IMURPC::DeserializeResponce(const SerialInterface::InPackage &package)
  {
    assert(package.ResponceSize == IMUInfo::Size);
    const uint8_t *ptr = package.Data;
    return IMUInfo::DeserializeFrom(&ptr);
  }

  template <>
  IMURPC::Empty
  IMURPC::DeserializeResponce(const SerialInterface::InPackage &package)
  {
    assert(package.ResponceSize == Empty::Size);
    return Empty{};
  }

  template <typename T, typename ResponceT>
  bool IMURPC::PerformRPC(SerialInterface &si, T request, ResponceT &responce)
  {
    static_assert(std::is_same_v<ResponceT, typename T::ResponceType>, "Bad instantiation");

    SerialInterface::OutPackage outPackage = CreatePackage(request);

    if (!si.Send(outPackage))
    {
      HasError = true;
      Error = si.GetError();
      return false;
    }

    SerialInterface::InPackage inPackage;
    if (!si.Receive(T::ResponceType::Size, inPackage))
    {
      HasError = true;
      Error = si.GetError();
      return false;
    }

    if (inPackage.Error != 0)
    {
      HasError = true;
      Error = GetErrorDescription(inPackage.Error);
      return false;
    }

    responce = DeserializeResponce<typename T::ResponceType>(inPackage);
    return true;
  }

  template <>
  IMURPC::Byte
  IMURPC::DeserializeResponce(const SerialInterface::InPackage &package)
  {
    assert(package.ResponceSize == Byte::Size);
    return {package.Data[0]};
  }

  bool IMURPC::IsOk() const { return !HasError; }
  std::string IMURPC::GetError() const { return Error; }

  const char *IMURPC::GetErrorDescription(ErrorCodes::Type errCode)
  {
    switch (errCode)
    {
    case ErrorCodes::Success:
      return "Success";
    case ErrorCodes::FrameUnavailable:
      return "Frame unavailable";
    case ErrorCodes::UnknownMode:
      return "Unknown Mode";
    case ErrorCodes::BadRequest:
      return "Bad Request";
    case ErrorCodes::BadOffset:
      return "Bad offset";
    default:
      return "Unknown error";
    }
  }

  IMURPC::IMUInfo IMURPC::IMUInfo::DeserializeFrom(uint8_t const **data)
  {
    assert(data);
    assert(*data);

    const uint8_t *ptr = *data;

    IMUInfo info;
    info.First = *reinterpret_cast<const uint16_t *>(ptr);
    ptr += sizeof(uint16_t);

    info.NumAv = *reinterpret_cast<const uint16_t *>(ptr);
    ptr += sizeof(uint16_t);

    info.MaxFrames = *reinterpret_cast<const uint16_t *>(ptr);
    ptr += sizeof(uint16_t);

    return info;
  }

  IMURPC::IMUFrame IMURPC::IMUFrame::DeserializeFrom(uint8_t const **data)
  {
    assert(data);
    assert(*data);

    const uint8_t *ptr = *data;

    IMUFrame fr;
    // cppcheck-suppress uninitStructMember
    auto &qt = fr.Orientation;

    // cppcheck-suppress uninitStructMember
    auto &ts = fr.Timestamp;

    const float norm = 16384; // 2**14

    qt.X = float(*reinterpret_cast<const int16_t *>(ptr)) / norm;
    ptr += sizeof(int16_t);

    qt.Y = float(*reinterpret_cast<const int16_t *>(ptr)) / norm;
    ptr += sizeof(int16_t);

    qt.Z = float(*reinterpret_cast<const int16_t *>(ptr)) / norm;
    ptr += sizeof(int16_t);

    qt.W = float(*reinterpret_cast<const int16_t *>(ptr)) / norm;
    ptr += sizeof(int16_t);

    /*
    qt.Accuracy = *reinterpret_cast<const float *>(ptr);
    ptr += sizeof(float);
    */

    ts.TimeS = *reinterpret_cast<const uint32_t *>(ptr);
    ptr += sizeof(uint32_t);

    ts.TimeNS = *reinterpret_cast<const uint32_t *>(ptr);
    ptr += sizeof(uint32_t);

    fr.SensorID = *reinterpret_cast<const uint8_t *>(ptr);
    ptr += sizeof(uint8_t);

    return fr;
  }

  template bool IMURPC::PerformRPC(SerialInterface &, IMUFrameRequest, IMUFrameRequest::ResponceType &);
  template bool IMURPC::PerformRPC(SerialInterface &, IMUInfoRequest, IMUInfoRequest::ResponceType &);
  template bool IMURPC::PerformRPC(SerialInterface &, IMULatestRequest, IMULatestRequest::ResponceType &);
  template bool IMURPC::PerformRPC(SerialInterface &, IMUResetRequest, IMUResetRequest::ResponceType &);
  template bool IMURPC::PerformRPC(SerialInterface &, SetStrobeOffsetRequest, SetStrobeOffsetRequest::ResponceType &);
  template bool IMURPC::PerformRPC(SerialInterface &, StrobeWidthRequest, StrobeWidthRequest::ResponceType &);
  template bool IMURPC::PerformRPC(SerialInterface &, ConfigureFilterRequest, ConfigureFilterRequest::ResponceType &);

} // namespace Roki