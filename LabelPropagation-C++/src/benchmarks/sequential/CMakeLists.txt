add_executable(benchmark_sequential benchmark_sequential.cpp)

target_link_libraries(benchmark_sequential label_propagation)

set(CMAKE_CXX_FLAGS "-O3")

