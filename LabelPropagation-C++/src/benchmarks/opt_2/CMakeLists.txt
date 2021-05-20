add_executable(benchmark_opt_2 benchmark_opt_2.cpp)

include_directories("../../code/opt_2")
target_link_libraries(benchmark_opt_2 label_propagation_opt2)

set(CMAKE_CXX_FLAGS "-O3")

# set(CMAKE_CXX_FLAGS " -g -fopenmp")

