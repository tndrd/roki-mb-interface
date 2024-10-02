#include "MotherboardAdapter_TMP.hpp"

namespace Roki
{

  using MA = MotherboardAdapter;
  using TMP = MotherboardAdapter_TMP;



  #define RET_IMPL(call) \
	auto ret = MA::call;\
  	if (std::get<0>(ret) == false) throw std::runtime_error(MA::GetError());\
  	return std::get<1>(ret);


  #define VOID_IMPL(call) \
  auto ret = MA::call;\
  if (ret == false) throw std::runtime_error(MA::GetError());\

  IMUFrame TMP::GetIMUFrame(int seq)
  {
    RET_IMPL(GetIMUFrame(seq));
  }

  FrameContainerInfo TMP::GetIMUContainerInfo()
  {
    RET_IMPL(GetIMUContainerInfo());
  }

  void  TMP::ResetStrobeContainers()
  {
    VOID_IMPL(ResetStrobeContainers());
  }

  void TMP::ResetBodyQueue() {
    VOID_IMPL(ResetBodyQueue());
  }

  void TMP::SetIMUStrobeOffset(int offset)
  {
    VOID_IMPL(SetIMUStrobeOffset(offset));
  }

  IMUFrame TMP::GetIMULatest()
  {
    RET_IMPL(GetIMULatest());
  }

  int TMP::GetStrobeWidth()
  {
    RET_IMPL(GetStrobeWidth());
  }

  void TMP::ConfigureStrobeFilter(int targetDuration, int durationThreshold)
  {
    VOID_IMPL(ConfigureStrobeFilter(targetDuration, durationThreshold));
  }

  BodyQueueInfo TMP::GetBodyQueueInfo()
  {
    RET_IMPL(GetBodyQueueInfo());
  }

  void TMP::SetBodyQueuePeriod(int periodMs)
  {
    VOID_IMPL(SetBodyQueuePeriod(periodMs));
  }

  Version TMP::GetVersion()
  {
    RET_IMPL(GetVersion());
  }

 } // namespace Roki
