#pragma once
#include <string>

namespace MbInterface {
class MbError {
private:
  mutable bool HasError = false;
  mutable std::string Error = "";

  std::string Prefix;

protected:
  bool MakeError(const std::string &msg) const;
  bool MakePrefixError(const std::string &prefix, const std::string &msg) const;
  bool MakeSuccess() const;

public:
  MbError(const std::string &prefix);

  bool IsOk() const;
  std::string GetError() const;

  MbError(const MbError &) = default;
  MbError &operator=(const MbError &) = default;

  MbError(MbError &&) = default;
  MbError &operator=(MbError &&) = default;

  virtual ~MbError() = default;
};
} // namespace MbInterface