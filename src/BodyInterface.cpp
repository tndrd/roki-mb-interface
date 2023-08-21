#include "BodyInterface.hpp"

namespace Roki {

auto BodyInterface::MessageMode::Serialize(MessageMode::Type mode) -> uint8_t {
  return mode;
}

auto BodyInterface::MessageMode::Deserialize(uint8_t data) -> Type {
  return data;
}

bool BodyInterface::IsOk() const { return !HasError; }
std::string BodyInterface::GetError() const { return Error; }

bool BodyInterface::Send(MBInterface &mbi, Request request) {
  MBInterface::OutPackage out{
      MBInterface::Periphery::Body, request.ResponceSize,
      MessageMode::Serialize(request.Mode), request.Data, request.RequestSize};

  if (!mbi.Send(out)) {
    HasError = true;
    Error = mbi.GetError();
    return false;
  }

  return true;
}

bool BodyInterface::Recieve(MBInterface &mbi, Responce &responce,
                            uint8_t responceSize) {
  MBInterface::InPackage package;

  if (!mbi.Receive(responceSize, package)) {
    HasError = true;
    Error = mbi.GetError();
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