cmake_minimum_required(VERSION 3.8)
project(RokiMotherboard)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Werror")

add_subdirectory(deps/pybind11)
add_subdirectory(deps/rcb4-base-class)
add_subdirectory(deps/roki-mb-service)
add_subdirectory(deps/googletest)

add_definitions(-DDBGMB=1)

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

add_library(MotherboardAdapter src/MotherboardAdapter.cpp)
target_link_libraries(MotherboardAdapter PUBLIC Motherboard)

add_library(RokiRcb4Adapter src/RokiRcb4Adapter.cpp)
target_link_libraries(RokiRcb4Adapter PUBLIC RokiRcb4)

add_library(MbDefaultConfig src/MbDefaultConfig.cpp)
target_link_libraries(MbDefaultConfig PUBLIC Motherboard)

include(tests/tests.cmake)
include(bindings/bindings.cmake)
# include(examples/examples.cmake)
