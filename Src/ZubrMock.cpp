#include "Zubr.hpp"

using namespace Roki;

#define MOCK(ret) {return ret;}

bool Zubr::MakeError(const std::string &msg) const MOCK(false);

bool Zubr::MakePrefixError(const std::string &prefix,
                           const std::string &msg) const MOCK(false);

Zubr::Zubr(Motherboard &mb) : Mboard{&mb} {}

bool Zubr::IsOk() const MOCK(true);
std::string Zubr::GetError() const MOCK("OK");

uint8_t Zubr::EvalCheckSum() const MOCK(0);

void Zubr::PackGetCmd(AddrT addr) MOCK(;);

bool Zubr::UnPackGetCmd(AddrT addr, const uint8_t **dataPtr) const MOCK(true);

void Zubr::PackSetCmd(AddrT addr, const uint8_t *data) MOCK(;);

bool Zubr::UnPackSetCmd() const MOCK(true);

bool Zubr::Synchronize(uint8_t responceSize) MOCK(true);