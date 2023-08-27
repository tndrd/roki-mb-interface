#include "BodyRPC.hpp"

namespace Roki {

auto BodyRPC::MessageMode::Serialize(MessageMode::Type mode) -> uint8_t {
  return mode;
}

auto BodyRPC::MessageMode::Deserialize(uint8_t data) -> Type {
  return data;
}

bool BodyRPC::IsOk() const { return !HasError; }
std::string BodyRPC::GetError() const { return Error; }

bool BodyRPC::Send(SerialInterface &si, Request request) {
  SerialInterface::OutPackage out{
      SerialInterface::Periphery::Body, request.ResponceSize,
      MessageMode::Serialize(request.Mode), request.Data, request.RequestSize};

  if (!si.Send(out)) {
    HasError = true;
    Error = si.GetError();
    return false;
  }

  return true;
}

bool BodyRPC::Recieve(SerialInterface &si, Responce &responce,
                            uint8_t responceSize) {
  SerialInterface::InPackage package;

  if (!si.Receive(responceSize, package)) {
    HasError = true;
    Error = si.GetError();
    return false;
  }

  if (package.Error != 0) {
    HasError = true;
    Error = "Body request failed";
  }

  responce.Data = package.Data;
  responce.ResponceSize = package.ResponceSize;
  responce.Mode = MessageMode::Deserialize(package.MetaInfo);

  return true;
}

} // namespace Roki