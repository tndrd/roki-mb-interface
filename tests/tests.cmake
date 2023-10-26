add_executable(Test tests/Test.cpp)
target_link_libraries(Test GTest::gtest_main Motherboard RokiRcb4 MbDefaultConfig)