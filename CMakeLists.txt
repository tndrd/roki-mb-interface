cmake_minimum_required(VERSION 3.8)
project(RokiMotherboard)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(deps/pybind11)
add_subdirectory(deps/rcb4-base-class)

#ADD_DEFINITIONS(-DDBGMB=1)

include_directories(inc/)

add_library(Helpers src/Helpers.cpp)

add_library(SerialInterface src/SerialInterface.cpp)
target_link_libraries(SerialInterface Helpers)

add_library(IMURPC src/IMURPC.cpp)
target_link_libraries(IMURPC PUBLIC SerialInterface)

add_library(BodyRPC src/BodyRPC.cpp)
target_link_libraries(BodyRPC PUBLIC SerialInterface)

add_library(Motherboard src/Motherboard.cpp)
target_link_libraries(Motherboard PUBLIC IMURPC BodyRPC)

add_library(RokiRcb4 src/RokiRcb4.cpp)
target_link_libraries(RokiRcb4 PUBLIC rcb4base Motherboard)

add_library(MotherboardAdapter src/MotherboardAdapter.cpp)
target_link_libraries(MotherboardAdapter PUBLIC Motherboard)

# Tests

include(bindings/bindings.cmake)
include(tests/tests.cmake)