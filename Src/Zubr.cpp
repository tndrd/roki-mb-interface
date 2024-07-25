#ifdef USE_MB_MOCK
#pragma message "USE_MB_MOCK is set, building using Zubr mock"
#include "ZubrMock.cpp"
#else
#pragma message "USE_MB_MOCK is not set, Zubr building normally"
#include "ZubrImpl.cpp"
#endif
