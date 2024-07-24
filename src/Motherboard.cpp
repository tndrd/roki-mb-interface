#ifdef USE_MB_MOCK
#pragma message "USE_MB_MOCK is set, building using motherboard mock"
#include "MotherboardMock.cpp"
#else
#pragma message "USE_MB_MOCK is not set, motherboard building normally"
#include "MotherboardImpl.cpp"
#endif
