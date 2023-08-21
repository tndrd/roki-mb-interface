#include "MotherboardInterface.hpp"

namespace Roki {
class BodyInterface {
  bool HasError = false;
  std::string Error;

public:
  struct MessageMode {
    using Type = uint8_t;

    static constexpr Type Forward = 0;
    static constexpr Type Ignore = 1;
    static constexpr Type Notify = 2;
    static constexpr Type Priority = 3;

    static uint8_t Serialize(MessageMode::Type mode) { return mode; }

    static MessageMode::Type Deserialize(uint8_t data) { return data; }
  };

  struct Request {
    const uint8_t* Data;
    uint8_t RequestSize;
    uint8_t ResponceSize;
    MessageMode::Type Mode;
  };

  struct Responce {
    const uint8_t* Data;
    uint8_t ResponceSize;
    MessageMode::Type Mode;
  };

public:
  bool Send(MBInterface &mbi, Request request) {
    MBInterface::OutPackage out {MBInterface::Periphery::Body, request.ResponceSize,
                       MessageMode::Serialize(request.Mode), request.Data, request.RequestSize};

    if (!mbi.Send(out)) {
      HasError = true;
      Error = mbi.GetError();
      return false;
    }

    return true;
  }

  bool Recieve(uint8_t responceSize, MBInterface& mbi, Responce& responce) {
    MBInterface::InPackage package;
    
    if (!mbi.Receive(responceSize, package)) {
      HasError = true;
      Error = mbi.GetError();
      return false;
    }

    if (package.Error != 0) {
      HasError = true;
      Error = "Body request failed";
    }

    responce.Data = package.Data;
    responce.ResponceSize = package.ResponceSize;
    responce.Mode = MessageMode::Deserialize(package.MetaInfo);

    return true;
  }
};

} // namespace Roki