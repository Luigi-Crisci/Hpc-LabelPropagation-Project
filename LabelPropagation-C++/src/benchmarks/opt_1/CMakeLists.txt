add_executable(benchmark_opt_1 benchmark_opt_1.cpp)

include_directories("../../code/opt_1")

target_link_libraries(benchmark_opt_1 label_propagation_opt1)

set(CMAKE_CXX_FLAGS "-O3")

set(CMAKE_CXX_FLAGS "-fopenmp")

