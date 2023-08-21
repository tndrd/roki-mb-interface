cmake_minimum_required(VERSION 3.0)
project(RokiMotherboard)

include_directories(inc/)

add_library(MBInterface src/MBInterface.cpp)

add_library(IMUInterface src/IMUInterface.cpp)
target_link_libraries(IMUInterface PUBLIC MBInterface)

add_library(BodyInterface src/BodyInterface.cpp)
target_link_libraries(BodyInterface PUBLIC MBInterface)