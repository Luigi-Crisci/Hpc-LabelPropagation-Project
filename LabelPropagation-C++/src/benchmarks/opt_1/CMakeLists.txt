add_executable(benchmark_opt_1 benchmark_opt_1.cpp)

target_link_libraries(benchmark_opt_1 label_propagation_opt1)

set(CMAKE_CXX_FLAGS "-O3")

