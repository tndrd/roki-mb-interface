#pragma once
#include "BodyInterface.hpp"

namespace Roki {

class BodySingleSender {
private:
  bool HasError = false;
  std::string Error;

private:
  bool MakeError(const std::string &msg) {
    HasError = true;
    Error = msg;
    return false;
  }

public:
  bool Send(MBInterface &mbi, BodyInterface &body, const uint8_t *requestData,
            uint8_t *responceData, uint8_t requestSize, uint8_t responceSize) {
    
    BodyInterface::Request request;
    request.Data = requestData;
    request.RequestSize = requestSize;
    request.ResponceSize = responceSize;
    request.Mode = BodyInterface::MessageMode::Priority;
    
    if (!body.Send(mbi, request)) {
      HasError = true;
      Error = body.GetError();
      return false;
    }

    BodyInterface::Responce responce;
    if (!body.Recieve(mbi, responce, responceSize)) {
      HasError = true;
      Error = body.GetError();
    }

    return true;
  }
};

} // namespace Roki