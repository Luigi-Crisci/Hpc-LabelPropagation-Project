add_executable(benchmark_opt_3 benchmark_opt_3.cpp)

include_directories("../../code/opt_3")
target_link_libraries(benchmark_opt_3 label_propagation_opt3)

set(CMAKE_CXX_FLAGS "-O3 -DDEBUG")

# set(CMAKE_CXX_FLAGS "-fopenmp")

