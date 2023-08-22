add_executable(TestIMU tests/TestIMU.cpp)
target_link_libraries(TestIMU PRIVATE MBInterface IMUInterface pthread)