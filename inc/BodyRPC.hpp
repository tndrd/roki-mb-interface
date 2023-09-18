#pragma once

#include "SerialInterface.hpp"

namespace Roki
{
  class BodyRPC
  {
  public:
    struct MessageMode
    {
      using Type = uint8_t;

      static constexpr Type Sync = 0;
      static constexpr Type Async = 1;
      static constexpr Type Info = 2;
      static constexpr Type SetPeriod = 3;

      static uint8_t Serialize(MessageMode::Type mode);
      static Type Deserialize(uint8_t data);
    };

    struct ErrorCode final
    {
      using Type = uint8_t;
      static constexpr Type Success = 0;
      static constexpr Type Timeout = 1;
      static constexpr Type NACK = 2;
      static constexpr Type Unknown = 3;
      static constexpr Type BadPeriod = 4;

      static uint8_t Serialize(Type mode);
      static Type Deserialize(uint8_t val);

      static std::string GetDescription(Type error);
    };

    struct Info
    {
      uint16_t NumRequests;
      uint16_t NumResponces;

      static constexpr size_t Size = 2 * sizeof(uint16_t);

      static Info DeserializeFrom(uint8_t const**ptr);
    };

    struct SetPeriodRequest {
      uint8_t Value;

      static constexpr size_t Size = 1;
    };

    struct Request
    {
      const uint8_t *Data;
      uint8_t RequestSize;
      uint8_t ResponceSize;
      MessageMode::Type Mode;
    };

    struct Responce
    {
      const uint8_t *Data;
      uint8_t ResponceSize;
      MessageMode::Type Mode;
      ErrorCode::Type Error;
    };

  private:
    bool HasError = false;
    std::string Error;

    bool MakeError(const std::string &msg);

  public:
    bool IsOk() const;
    std::string GetError() const;

    bool Send(SerialInterface &si, Request request);
    bool Recieve(SerialInterface &si, Responce &responce, uint8_t responceSize);
    bool GetInfo(SerialInterface& si, Info& result);
    bool SetPeriod(SerialInterface& si, uint8_t periodMs);
  };

} // namespace Roki