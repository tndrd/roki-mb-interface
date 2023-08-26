add_executable(TestIMU tests/TestIMU.cpp)
target_link_libraries(TestIMU PRIVATE MBInterface IMUInterface pthread)

add_executable(TestBody tests/TestBody.cpp)
target_link_libraries(TestBody PRIVATE MBInterface BodyInterface pthread)