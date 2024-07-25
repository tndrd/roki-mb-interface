cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Werror")

if(DEFINED ENV{ROKI_USE_HARDWARE_MOCKS})
  set(USE_MB_MOCK ON)
else()
  set(USE_MB_MOCK OFF)
endif()

set(DEBUG_MB OFF)

project(RokiMotherboard)

add_subdirectory(Deps/rcb4-base-class)
add_subdirectory(Deps/roki-mb-service)

if(USE_MB_MOCK)
  message(INFO " Using hardware mocks")
  add_library(rcb4base ALIAS rcb4base_mock)
  add_compile_definitions(USE_MB_MOCK)
else()
  message(INFO " Using hardware implementation")
  add_library(rcb4base ALIAS rcb4base_impl)
endif()

if(DEBUG_MB)
  add_compile_definitions(DBGMB)
endif()

include_directories(Inc/)

add_library(DescriptorWrapper Src/Helpers/DescriptorWrapper.cpp)

add_library(MbSerial Src/MbSerial.cpp)
target_link_libraries(MbSerial PUBLIC DescriptorWrapper)

add_library(MbClient Src/MbClient.cpp)
target_link_libraries(MbClient PUBLIC MbSerial)

add_library(IMUFrame Src/Helpers/IMUFrame.cpp)
target_link_libraries(IMUFrame PUBLIC MbService)

add_library(Motherboard Src/Motherboard.cpp)
target_link_libraries(Motherboard PUBLIC MbService MbClient IMUFrame)

add_library(RokiRcb4 Src/RokiRcb4.cpp)
target_link_libraries(RokiRcb4 PUBLIC rcb4base Motherboard)

add_library(Zubr Src/Zubr.cpp)
target_link_libraries(Zubr PUBLIC Motherboard)

add_library(MotherboardAdapter Src/MotherboardAdapter.cpp)
target_link_libraries(MotherboardAdapter PUBLIC Motherboard)

add_library(RokiRcb4Adapter Src/RokiRcb4Adapter.cpp)
target_link_libraries(RokiRcb4Adapter PUBLIC RokiRcb4)

add_library(ZubrAdapter Src/ZubrAdapter.cpp)
target_link_libraries(ZubrAdapter PUBLIC Zubr)