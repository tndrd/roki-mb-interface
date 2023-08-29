set(CMAKE_CXX_FLAGS "-fPIC")

pybind11_add_module(Roki bindings/MBBinding.cpp)
target_link_libraries(Roki PUBLIC MotherboardAdapter)