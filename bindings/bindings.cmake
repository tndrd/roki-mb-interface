set(CMAKE_CXX_FLAGS "-fPIC")

pybind11_add_module(Roki bindings/MBBinding.cpp)
target_link_libraries(Roki PUBLIC MotherboardAdapter RokiRcb4Adapter MbDefaultConfig ZubrAdapter)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

message(INFO "Python site-packages directory: ${Python3_SITEARCH}")

install(TARGETS Roki DESTINATION ${Python3_SITEARCH})