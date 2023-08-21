#pragma once

#include "MBInterface.hpp"

namespace Roki {
class BodyInterface {
public:
  struct MessageMode {
    using Type = uint8_t;

    static constexpr Type Forward = 0;
    static constexpr Type Ignore = 1;
    static constexpr Type Notify = 2;
    static constexpr Type Priority = 3;

    static uint8_t Serialize(MessageMode::Type mode);
    static MessageMode::Type Deserialize(uint8_t data);
  };

  struct Request {
    const uint8_t *Data;
    uint8_t RequestSize;
    uint8_t ResponceSize;
    MessageMode::Type Mode;
  };

  struct Responce {
    const uint8_t *Data;
    uint8_t ResponceSize;
    MessageMode::Type Mode;
  };

private:
  bool HasError = false;
  std::string Error;

public:
  bool IsOk() const;
  std::string GetError() const;

  bool Send(MBInterface &mbi, Request request);
  bool Recieve(MBInterface &mbi, Responce &responce, uint8_t responceSize);
};

} // namespace Roki