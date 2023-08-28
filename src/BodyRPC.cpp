#include "BodyRPC.hpp"

namespace Roki
{

  auto BodyRPC::MessageMode::Serialize(MessageMode::Type mode) -> uint8_t
  {
    return mode;
  }

  auto BodyRPC::MessageMode::Deserialize(uint8_t data) -> Type
  {
    return data;
  }

  bool BodyRPC::MakeError(const std::string &msg)
  {
    HasError = true;
    Error = msg;
    return false;
  }

  auto BodyRPC::ErrorCode::Serialize(Type mode) -> uint8_t { return mode; }
  auto BodyRPC::ErrorCode::Deserialize(uint8_t val) -> Type { return val; }

  std::string BodyRPC::ErrorCode::GetDescription(Type error)
  {
    switch (error)
    {
    case Success:
      return "Success";
    case Timeout:
      return "Body timeout";
    case Unknown:
      return "Unknow body error";
    default:
      return "Unknown error value";
    }
  }

  bool BodyRPC::IsOk() const { return !HasError; }
  std::string BodyRPC::GetError() const { return Error; }

  bool BodyRPC::Send(SerialInterface &si, Request request)
  {
    SerialInterface::OutPackage out{
        SerialInterface::Periphery::Body, request.ResponceSize,
        MessageMode::Serialize(request.Mode), request.Data, request.RequestSize};

    if (!si.Send(out))
      return MakeError(si.GetError());

    return true;
  }

  bool BodyRPC::Recieve(SerialInterface &si, Responce &responce,
                        uint8_t responceSize)
  {
    SerialInterface::InPackage package;

    if (!si.Receive(responceSize, package))
      return MakeError(si.GetError());

    ErrorCode::Type error = ErrorCode::Deserialize(package.Error);

    if (error != ErrorCode::Success)
      return MakeError(ErrorCode::GetDescription(error));

    responce.Data = package.Data;
    responce.ResponceSize = package.ResponceSize;
    responce.Error = error;
    responce.Mode = MessageMode::Deserialize(package.MetaInfo);

    return true;
  }

  BodyRPC::Info BodyRPC::Info::DeserializeFrom(uint8_t const **ptr)
  {
    Info info;

    assert(ptr);
    assert(*ptr);

    info.NumRequests = *reinterpret_cast<const uint16_t *>(*ptr);
    *ptr += sizeof(uint16_t);

    info.NumResponces = *reinterpret_cast<const uint16_t *>(*ptr);
    *ptr += sizeof(uint16_t);

    return info;
  }

  bool BodyRPC::GetInfo(SerialInterface& si, Info& result) {
    std::array<uint8_t, 1> buf {0};
    SerialInterface::OutPackage out{
        SerialInterface::Periphery::Body, Info::Size,
        MessageMode::Serialize(MessageMode::Info), buf.data(), buf.size()};

    if (!si.Send(out))
      return MakeError(si.GetError());

    SerialInterface::InPackage package;

    if (!si.Receive(Info::Size, package))
      return MakeError(si.GetError());

    const uint8_t* ptr = package.Data;
    result = Info::DeserializeFrom(&ptr);
    return true;
  }

} // namespace Roki