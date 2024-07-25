cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Werror")

if(DEFINED ENV{ROKI_USE_HARDWARE_MOCKS})
  set(USE_MB_MOCK ON)
else()
  set(USE_MB_MOCK OFF)
endif()

project(RokiMotherboard)

add_subdirectory(deps/rcb4-base-class)
add_subdirectory(deps/roki-mb-service)

if(USE_MB_MOCK)
  message(INFO " Using hardware mocks")
  add_library(rcb4base ALIAS rcb4base_mock)
  add_compile_definitions(USE_MB_MOCK)
else()
  message(INFO " Using hardware implementation")
  add_library(rcb4base ALIAS rcb4base_impl)
endif()

#add_definitions(-DDBGMB=1)

include_directories(inc/)

add_library(DescriptorWrapper src/Helpers/DescriptorWrapper.cpp)

add_library(MbSerial src/MbSerial.cpp)
target_link_libraries(MbSerial PUBLIC DescriptorWrapper)

add_library(MbClient src/MbClient.cpp)
target_link_libraries(MbClient PUBLIC MbSerial)

add_library(IMUFrame src/Helpers/IMUFrame.cpp)
target_link_libraries(IMUFrame PUBLIC MbService)

add_library(Motherboard src/Motherboard.cpp)
target_link_libraries(Motherboard PUBLIC MbService MbClient IMUFrame)

add_library(RokiRcb4 src/RokiRcb4.cpp)
target_link_libraries(RokiRcb4 PUBLIC rcb4base Motherboard)

add_library(Zubr src/Zubr.cpp)
target_link_libraries(Zubr PUBLIC Motherboard)

add_library(MotherboardAdapter src/MotherboardAdapter.cpp)
target_link_libraries(MotherboardAdapter PUBLIC Motherboard)

add_library(RokiRcb4Adapter src/RokiRcb4Adapter.cpp)
target_link_libraries(RokiRcb4Adapter PUBLIC RokiRcb4)

add_library(ZubrAdapter src/ZubrAdapter.cpp)
target_link_libraries(ZubrAdapter PUBLIC Zubr)