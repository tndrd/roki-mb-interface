add_executable(TestIMU tests/TestIMU.cpp)
target_link_libraries(TestIMU PRIVATE Motherboard pthread)

add_executable(TestBody tests/TestBody.cpp)
target_link_libraries(TestBody PRIVATE Motherboard Rcb4Adapter pthread)

add_executable(TestAdapter tests/TestAdapter.cpp)
target_link_libraries(TestAdapter PRIVATE MotherboardAdapter Rcb4Adapter pthread)