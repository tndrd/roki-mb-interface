#pragma once

#include "SerialInterface.hpp"
#include <type_traits>

namespace Roki
{
  class MotherboardRPC
  {
  public:
    struct ProcedureID
    {
      using Type = uint8_t;
      static constexpr Type IMUFrame = 0;
      static constexpr Type BodyFrame = 1;
      static constexpr Type IMUInfo = 2;
      static constexpr Type BodyInfo = 3;
      static constexpr Type IMULatest = 4;
      static constexpr Type ResetContainers = 5;
      static constexpr Type IMUStrobeOffset = 6;
      static constexpr Type BodyStrobeOffset = 7;
      static constexpr Type GetStrobeWidth = 8;
      static constexpr Type ConfigureStrobeFilter = 9;
      static constexpr Type GetVersion = 10;

      static uint8_t Serialize(Type mode)
      {
        return mode;
      }
      static Type Deserialize(uint8_t meta)
      {
        return meta;
      }
    };

    struct ErrorCodes
    {
      using Type = uint8_t;
      static constexpr Type Success = 0;
      static constexpr Type FrameUnavailable = 1;
      static constexpr Type UnknownMode = 2;
      static constexpr Type BadRequest = 3;
      static constexpr Type BadOffset = 4;
      static constexpr Type TargetError = 5;
    };

    struct Messages
    {
      struct IMUFrame
      {
        struct TimestampT
        {
          uint32_t TimeS;
          uint32_t TimeNS;
        };

        struct Quaternion
        {
          float X;
          float Y;
          float Z;
          float W;
        };

        Quaternion Orientation;
        TimestampT Timestamp;

        uint8_t SensorID;

        static constexpr size_t Size = 4 * sizeof(int16_t) + /* sizeof(float) + */
                                       2 * sizeof(uint32_t) +
                                       sizeof(uint8_t);

        static IMUFrame Deserialize(uint8_t const **ptr);
      };

      struct BodyFrame
      {
        std::vector<uint8_t> ServoData;

        static constexpr size_t Size = 33;
        static BodyFrame Deserialize(uint8_t const **ptr);
      };

      struct FrameContainerInfo
      {
        uint16_t First;
        uint16_t NumAv;
        uint16_t MaxFrames;

        static constexpr size_t Size = 3 * sizeof(uint16_t);
        static FrameContainerInfo Deserialize(uint8_t const **ptr);
      };

      struct Empty
      {
        static constexpr size_t Size = 1;

        void Serialize(uint8_t **ptr);
        static Empty Deserialize(uint8_t const **ptr);
      };

      struct Byte
      {
        static constexpr size_t Size = 1;
        uint8_t Value;

        void Serialize(uint8_t **ptr);
        static Empty Deserialize(uint8_t const **ptr);
      };

      struct FrameNumber
      {
        static constexpr size_t Size = 2;
        uint16_t Seq;

        void Serialize(uint8_t **ptr);
      };

      struct StrobeFilterConfig
      {
        static constexpr size_t Size = 2;

        void Serialize(uint8_t **ptr);
      };
    };

    struct Procedures
    {
      struct GetIMUFrame
      {
        static constexpr ProcedureID::Type ID = ProcedureID::IMUFrame;

        using Request = Messages::FrameNumber;
        using Responce = Messages::IMUFrame;
      };

      struct GetBodyFrame
      {
        static constexpr ProcedureID::Type ID = ProcedureID::BodyFrame;

        using Request = Messages::FrameNumber;
        using Responce = Messages::BodyFrame;
      };

      struct IMUInfo
      {
        static constexpr ProcedureID::Type ID = ProcedureID::IMUInfo;

        using Request = Messages::Empty;
        using Responce = Messages::FrameContainerInfo;
      };

      struct BodyInfo
      {
        static constexpr ProcedureID::Type ID = ProcedureID::BodyInfo;

        using Request = Messages::Empty;
        using Responce = Messages::FrameContainerInfo;
      };

      struct IMULatest
      {
        static constexpr ProcedureID::Type ID = ProcedureID::IMULatest;

        using Request = Messages::Empty;
        using Responce = Messages::IMUFrame;
      };

      struct ResetContainers
      {
        static constexpr ProcedureID::Type ID = ProcedureID::ResetContainers;

        using Request = Messages::Empty;
        using Responce = Messages::Empty;
      };

      struct IMUStrobeOffset
      {
        static constexpr ProcedureID::Type ID = ProcedureID::IMUStrobeOffset;

        using RequestType = Messages::Byte;
        using ResponceType = Messages::Empty;
      };

      struct BodyStrobeOffset
      {
        static constexpr ProcedureID::Type ID = ProcedureID::BodyStrobeOffset;

        using RequestType = Messages::Byte;
        using ResponceType = Messages::Empty;
      };

      struct StrobeWidth
      {
        static constexpr ProcedureID::Type ID = ProcedureID::GetStrobeWidth;

        using RequestType = Messages::Empty;
        using ResponceType = Messages::Byte;
      };

      struct ConfigureFilter
      {
        static constexpr size_t Size = 2;
        using RequestType = Messages::StrobeFilterConfig;
        using ResponceType = Messages::Empty;
        static constexpr ProcedureID::Type Mode = ProcedureID::ConfigureFilter;

        uint8_t TargetDuration;
        uint8_t DurationThreshold;
      };
    }

    private : static constexpr size_t BufferSize = 32;
    std::array<uint8_t, BufferSize> RequestBuffer;

    bool HasError = false;
    std::string Error;

    void SerializeToBuf(IMUFrameRequest request);
    void SerializeToBuf(IMUInfoRequest request);
    void SerializeToBuf(IMULatestRequest request);
    void SerializeToBuf(IMUResetRequest request);
    void SerializeToBuf(SetStrobeOffsetRequest request);
    void SerializeToBuf(StrobeWidthRequest request);
    void SerializeToBuf(ConfigureFilterRequest request);

    template <typename T>
    SerialInterface::OutPackage CreatePackage(T Request);

    template <typename T>
    T DeserializeResponce(const SerialInterface::InPackage &package);

    static const char *GetErrorDescription(ErrorCodes::Type errCode);
    static bool ErrorIsFatal(ErrorCodes::Type errCode);

  public:
    template <typename T, typename ResponceT>
    bool PerformRPC(SerialInterface &si, T Request, ResponceT &responce);

    bool IsOk() const;
    std::string GetError() const;
  };
} // namespace Roki