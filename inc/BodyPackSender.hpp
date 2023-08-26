#pragma once
#include "BodyInterface.hpp"

namespace Roki {

class BodyPackSender {
private:
  std::vector<BodyInterface::Request> Requests;
  bool HasError = false;
  std::string Error;

private:
  bool MakeError(const std::string &msg) {
    HasError = true;
    Error = msg;
    return false;
  }

public:
  bool StartRequestPack() {
    Requests.clear();

    return true;
  }

  bool AddRequest(const uint8_t *Data, uint8_t RequestSize,
                  uint8_t ResponceSize) {
    BodyInterface::Request request;

    request.Data = Data;
    request.RequestSize = RequestSize;
    request.ResponceSize = ResponceSize;
    request.Mode = BodyInterface::MessageMode::Ignore;

    Requests.emplace_back(request);

    return true;
  }

  bool SendRequests(MBInterface &mbi, BodyInterface &body) {
    if (Requests.empty())
      return MakeError("Request pack is empty");

    Requests.back().Mode = BodyInterface::MessageMode::Notify;

    for (auto &request : Requests) {
      if (!body.Send(mbi, request)) {
        HasError = true;
        Error = body.GetError();
        return false;
      }
    }

    BodyInterface::Responce responce;
    body.Recieve(mbi, responce, 0);
  }
};

} // namespace Roki