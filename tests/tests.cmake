add_executable(RunTests tests/RunTests.cpp)
target_link_libraries(RunTests GTest::gtest_main Motherboard RokiRcb4 MbDefaultConfig)