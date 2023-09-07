add_executable(TestIMU examples/TestIMU.cpp)
target_link_libraries(TestIMU PRIVATE Motherboard pthread)

add_executable(TestBody examples/TestBody.cpp)
target_link_libraries(TestBody PRIVATE Motherboard RokiRcb4 pthread)

add_executable(TestBodyImu examples/TestBodyImu.cpp)
target_link_libraries(TestBodyImu PRIVATE Motherboard RokiRcb4 pthread)

add_executable(TestAdapter examples/TestAdapter.cpp)
target_link_libraries(TestAdapter PRIVATE MotherboardAdapter RokiRcb4 pthread)