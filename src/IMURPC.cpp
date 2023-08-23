#include "IMURPC.hpp"
namespace Roki
{

  void IMURPCStub::SerializeToBuf(IMUFrameRequest request)
  {
    *reinterpret_cast<uint16_t *>(RequestBuffer.data()) = request.seq;
  }

  void IMURPCStub::SerializeToBuf(IMUInfoRequest request)
  {
    RequestBuffer[0] = 0;
  }

  void IMURPCStub::SerializeToBuf(IMULatestRequest request)
  {
    RequestBuffer[0] = 0;
  }
  
  void IMURPCStub::SerializeToBuf(IMUResetRequest request)
  {
    RequestBuffer[0] = 0;
  }

  template <typename T>
  MBInterface::OutPackage IMURPCStub::CreatePackage(T Request)
  {
    SerializeToBuf(Request);
    MBInterface::OutPackage outPackage{MBInterface::Periphery::Imu, T::ResponceType::Size,
                                       RequestMode::Serialize(T::Mode), RequestBuffer.data(), T::Size};
    return outPackage;
  }

  template <>
  IMURPCStub::IMUFrame
  IMURPCStub::DeserializeResponce(const MBInterface::InPackage &package)
  {
    assert(package.ResponceSize == IMUFrame::Size);

    IMUFrame fr;
    // cppcheck-suppress uninitStructMember
    auto &qt = fr.Orientation;

    // cppcheck-suppress uninitStructMember
    auto &ts = fr.Timestamp;

    const uint8_t *ptr = package.Data;
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

  template <>
  IMURPCStub::IMUInfo
  IMURPCStub::DeserializeResponce(const MBInterface::InPackage &package)
  {
    assert(package.ResponceSize == IMUInfo::Size);

    IMUInfo fr;
    const uint8_t *ptr = package.Data;

    fr.First = *reinterpret_cast<const uint16_t *>(ptr);
    ptr += sizeof(uint16_t);

    fr.NumAv = *reinterpret_cast<const uint16_t *>(ptr);
    ptr += sizeof(uint16_t);

    return fr;
  }

  template <>
  IMURPCStub::Empty
  IMURPCStub::DeserializeResponce(const MBInterface::InPackage &package)
  {
    assert(package.ResponceSize == Empty::Size);
    return Empty{};
  }

  template <typename T, typename ResponceT>
  bool IMURPCStub::PerformRPC(MBInterface &mbi, T request, ResponceT &responce)
  {
    static_assert(std::is_same_v<ResponceT, typename T::ResponceType>, "Bad instantiation");

    MBInterface::OutPackage outPackage = CreatePackage(request);

    if (!mbi.Send(outPackage))
    {
      HasError = true;
      Error = mbi.GetError();
      return false;
    }

    MBInterface::InPackage inPackage;
    if (!mbi.Receive(T::ResponceType::Size, inPackage))
    {
      HasError = true;
      Error = mbi.GetError();
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

  bool IMURPCStub::IsOk() const { return !HasError; }
  std::string IMURPCStub::GetError() const { return Error; }

  const char *IMURPCStub::GetErrorDescription(ErrorCodes::Type errCode)
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
    default:
      return "Unknown error";
    }
  }

  template bool IMURPCStub::PerformRPC(MBInterface &, IMUFrameRequest, IMUFrameRequest::ResponceType &);
  template bool IMURPCStub::PerformRPC(MBInterface &, IMUInfoRequest, IMUInfoRequest::ResponceType &);
  template bool IMURPCStub::PerformRPC(MBInterface &, IMULatestRequest, IMULatestRequest::ResponceType &);
  template bool IMURPCStub::PerformRPC(MBInterface &, IMUResetRequest, IMUResetRequest::ResponceType &);

} // namespace Roki