#pragma once

#include "MbSerial.hpp"
#include <array>

namespace Roki {

struct MbClient {
private:
  static constexpr size_t BufferSize = 256;
  std::array<uint8_t, BufferSize> RequestBuffer;

  bool HasError = false;
  std::string Error;

private:
  template <typename Proc> using Request = typename Proc::RequestType;
  template <typename Proc> using Responce = typename Proc::ResponceType;
  template <typename Proc>
  using ErrorCodes = typename Proc::Service::ErrorCodes;

  template <typename Proc> using ErrorType = typename ErrorCodes<Proc>::Type;

private:
  bool MakeError(const std::string &msg) {
    HasError = true;
    Error = msg;
    return false;
  }

public:
  bool IsOk() const;
  std::string GetError() const;

  template <typename Proc>
  bool PerformRPC(MbSerial &serial, const Request<Proc> &request,
                  Responce<Proc> &responce) {
    MbSerial::OutPackage outPackage;
    MbSerial::InPackage inPackage;

    outPackage.ProcedureID = Proc::ID;
    outPackage.Data = RequestBuffer.data();
    outPackage.Size = request.GetPackedSize();

    request.Serialize(RequestBuffer.data());

    if (!serial.Send(outPackage))
      return MakeError(serial.GetError());

    if (!serial.Receive(inPackage))
      return MakeError(serial.GetError());

    ErrorType<Proc> error = ErrorCodes<Proc>::Deserialize(inPackage.Error);
    ErrorType<Proc> ok = ErrorCodes<Proc>::Success;

    if (error != ok)
      return MakeError(ErrorCodes<Proc>::GetDescription(error));

    responce = Responce<Proc>::Deserialize(inPackage.Data);
    return true;
  }
};
} // namespace Roki