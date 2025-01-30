#include "roki-mb-interface/Helpers/MbError.hpp"

namespace MbInterface {

MbError::MbError(const std::string &prefix) : Prefix{prefix} {}

bool MbError::IsOk() const { return !HasError; }
std::string MbError::GetError() const { return HasError ? Error : "Ok"; }

bool MbError::MakeError(const std::string &msg) const {
  HasError = true;
  Error = Prefix + ": " + msg;
  return false;
}

bool MbError::MakePrefixError(const std::string &prefix,
                              const std::string &msg) const {
  return MakeError(prefix + ": " + msg);
}

bool MbError::MakeSuccess() const {
  HasError = false;
  return true;
}

} // namespace MbInterface