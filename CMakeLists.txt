cmake_minimum_required(VERSION 3.8)
project(RokiMotherboard)

set(CMAKE_CXX_STANDARD 17)

include_directories(inc/)

add_library(Helpers src/Helpers.cpp)

add_library(MBInterface src/MBInterface.cpp)
target_link_libraries(MBInterface Helpers)

add_library(IMUInterface src/IMUInterface.cpp)
target_link_libraries(IMUInterface PUBLIC MBInterface)

add_library(BodyInterface src/BodyInterface.cpp)
target_link_libraries(BodyInterface PUBLIC MBInterface)

# Tests

include(tests/tests.cmake)