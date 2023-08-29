add_executable(TestIMU tests/TestIMU.cpp)
target_link_libraries(TestIMU PRIVATE Motherboard pthread)

add_executable(TestBody tests/TestBody.cpp)
target_link_libraries(TestBody PRIVATE Motherboard RokiRcb4 pthread)

add_executable(TestAdapter tests/TestAdapter.cpp)
target_link_libraries(TestAdapter PRIVATE MotherboardAdapter RokiRcb4 pthread)