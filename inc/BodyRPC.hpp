#pragma once

#include "SerialInterface.hpp"

namespace Roki {
class BodyRPC {
public:
  struct MessageMode {
    using Type = uint8_t;

    static constexpr Type Sync = 0;
    static constexpr Type Async = 1;

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

  bool Send(SerialInterface &si, Request request);
  bool Recieve(SerialInterface &si, Responce &responce, uint8_t responceSize);
};

} // namespace Roki