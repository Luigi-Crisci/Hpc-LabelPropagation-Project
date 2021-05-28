add_executable(benchmark_opt_4 benchmark_opt_4.cpp)

include_directories("../../code/opt_4")
target_link_libraries(benchmark_opt_4 label_propagation_opt4)

set(CMAKE_CXX_FLAGS "-O3 -DDEBUG")

# set(CMAKE_CXX_FLAGS "-fopenmp")

