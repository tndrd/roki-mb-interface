set(CMAKE_CXX_FLAGS "-fPIC")

pybind11_add_module(Roki bindings/MBBinding.cpp)
target_link_libraries(Roki PUBLIC MotherboardAdapter RokiRcb4Adapter)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

install(TARGETS Roki
        COMPONENT python
        LIBRARY DESTINATION "lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages"
        ARCHIVE DESTINATION "lib"
        RUNTIME DESTINATION "bin")