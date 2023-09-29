#include "MbService.hpp"

namespace Roki {

uint8_t MbService::ProcedureID::Serialize(Type id) { return id; }
auto MbService::ProcedureID::Deserialize(uint8_t val) -> Type {
  return val;
}

uint8_t MbService::ErrorCodes::Serialize(Type err) { return err; }
auto MbService::ErrorCodes::Deserialize(uint8_t val) -> Type {
  return val;
}

// Errorcodes
const char *
MbService::ErrorCodes::GetDescription(ErrorCodes::Type errCode) {
  switch (errCode) {
  case Success:
    return "Success";
  case FrameUnavailable:
    return "Requested frame is unavailable";
  case UnknownProcedure:
    return "Unknown procedure identifier";
  case BadRequest:
    return "Bad request";
  case BadStrobeOffset:
    return "Bad strobe offset";
  case BodyNACK:
    return "Command refused by body";
  case BodyUnknownError:
    return "Unknow body error";
  case BadBodyQueuePeriod:
    return "Bad body queue period";
  case BodyQueueFull:
    return "Body queue is full";
  default:
    return "Unknown error";
  }
}

} // namespace Roki