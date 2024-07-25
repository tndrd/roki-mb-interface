#include "roki-mb-interface/MbClient.hpp"

namespace MbInterface {
bool MbClient::IsOk() const { return HasError; }

std::string MbClient::GetError() const {
  if (HasError)
    return Error;
  return "OK";
}

} // namespace MbInterface