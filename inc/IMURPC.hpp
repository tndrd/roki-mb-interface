#pragma once

#include "SerialInterface.hpp"
#include <type_traits>

namespace Roki
{
  class IMURPC
  {
  public:
    struct RequestMode
    {
      using Type = uint8_t;
      static constexpr Type FrameBySeq = 0;
      static constexpr Type Info = 1;
      static constexpr Type LatestFrame = 2;
      static constexpr Type Reset = 3;
      static constexpr Type SetOffset = 4; 
      static constexpr Type StrobeWidth = 5;

      static uint8_t Serialize(Type mode) { return mode; }
      static Type Deserialize(uint8_t meta) { return meta; }
    };

    struct ErrorCodes
    {
      using Type = uint8_t;
      static constexpr Type Success = 0;
      static constexpr Type FrameUnavailable = 1;
      static constexpr Type UnknownMode = 2;
      static constexpr Type BadRequest = 3;
      static constexpr Type BadOffset = 4;
    };

    struct TimestampT
    {
      size_t TimeS;
      size_t TimeNS;
    };

    struct Quaternion
    {
      float X;
      float Y;
      float Z;
      float W;

      // float Accuracy;
    };

    struct IMUFrame
    {
      Quaternion Orientation;
      TimestampT Timestamp;

      uint8_t SensorID;

      static constexpr size_t Size = 4 * sizeof(int16_t) + /* sizeof(float) + */
                                     2 * sizeof(uint32_t) +
                                     sizeof(uint8_t);
      static IMUFrame DeserializeFrom(uint8_t const**ptr);
    };

    struct IMUInfo
    {
      uint16_t First;
      uint16_t NumAv;
      uint16_t MaxFrames;

      static constexpr size_t Size = 3 * sizeof(uint16_t);

      static IMUInfo DeserializeFrom(uint8_t const**ptr);
    };

    struct Empty
    {
      static constexpr size_t Size = 1;
    };

    struct Byte
    {
      static constexpr size_t Size = 1;
      uint8_t Value;
    };

    struct IMUFrameRequest
    {
      static constexpr size_t Size = 2;
      static constexpr RequestMode::Type Mode = RequestMode::FrameBySeq;
      using ResponceType = IMUFrame;

      uint16_t seq;
    };

    struct IMUInfoRequest
    {
      static constexpr size_t Size = 1;
      using ResponceType = IMUInfo;
      static constexpr RequestMode::Type Mode = RequestMode::Info;
    };

    struct IMULatestRequest
    {
      static constexpr size_t Size = 1;
      using ResponceType = IMUFrame;
      static constexpr RequestMode::Type Mode = RequestMode::LatestFrame;
    };

    struct IMUResetRequest
    {
      static constexpr size_t Size = 1;
      using ResponceType = Empty;
      static constexpr RequestMode::Type Mode = RequestMode::Reset;
    };

    struct SetStrobeOffsetRequest
    {
      static constexpr size_t Size = 1;
      using ResponceType = Empty;
      static constexpr RequestMode::Type Mode = RequestMode::SetOffset;

      uint8_t offset;
    };

    struct StrobeWidthRequest
    {
      static constexpr size_t Size = 1;
      using ResponceType = Byte;
      static constexpr RequestMode::Type Mode = RequestMode::StrobeWidth;
    };

  private:
    static constexpr size_t BufferSize = 32;
    std::array<uint8_t, BufferSize> RequestBuffer;

    bool HasError = false;
    std::string Error;

    void SerializeToBuf(IMUFrameRequest request);
    void SerializeToBuf(IMUInfoRequest request);
    void SerializeToBuf(IMULatestRequest request);
    void SerializeToBuf(IMUResetRequest request);
    void SerializeToBuf(SetStrobeOffsetRequest request);
    void SerializeToBuf(StrobeWidthRequest request);

    template <typename T>
    SerialInterface::OutPackage CreatePackage(T Request);

    template <typename T>
    T DeserializeResponce(const SerialInterface::InPackage &package);

    static const char *GetErrorDescription(ErrorCodes::Type errCode);

  public:
    template <typename T, typename ResponceT>
    bool PerformRPC(SerialInterface &si, T Request, ResponceT &responce);

    bool IsOk() const;
    std::string GetError() const;
  };
} // namespace Roki