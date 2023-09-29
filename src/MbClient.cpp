#include "MbClient.hpp"

namespace Roki {
bool MbClient::IsOk() const { return HasError; }

std::string MbClient::GetError() const {
  if (HasError)
    return Error;
  return "OK";
}

} // namespace Roki