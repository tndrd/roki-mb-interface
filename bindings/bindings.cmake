set(CMAKE_CXX_FLAGS "-fPIC")

pybind11_add_module(Roki bindings/MBBinding.cpp)
target_link_libraries(Roki PUBLIC MotherboardAdapter_TMP RokiRcb4Adapter ZubrAdapter Motherboard MbSerial)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# For unknown, reason cmake chooses python3.10, but we use 3.9
set(Python3_SITEARCH "/usr//lib/python3/dist-packages/")
message(INFO "Python site-packages directory: ${Python3_SITEARCH}")

install(TARGETS Roki DESTINATION ${Python3_SITEARCH})
