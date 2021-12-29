# Sequential
add_executable(benchmark_sequential benchmark.cpp)
target_include_directories(benchmark_sequential PUBLIC "../../code/sequential")
target_link_libraries(benchmark_sequential label_propagation)
#Opt 1
add_executable(benchmark_opt_1 benchmark.cpp utils.hpp)
target_include_directories(benchmark_opt_1 PUBLIC "../../code/opt_1")
target_link_libraries(benchmark_opt_1 label_propagation_opt1)
target_compile_options(benchmark_opt_1 PUBLIC "-O3")
#Opt 2
add_executable(benchmark_opt_2 benchmark.cpp utils.hpp)
target_include_directories(benchmark_opt_2 PUBLIC "../../code/opt_2")
target_link_libraries(benchmark_opt_2 label_propagation_opt2)
target_compile_options(benchmark_opt_2 PUBLIC "-O3")
#Opt 3
add_executable(benchmark_opt_3 benchmark.cpp utils.hpp)
target_include_directories(benchmark_opt_3 PUBLIC "../../code/opt_3")
target_link_libraries(benchmark_opt_3 label_propagation_opt3)
target_compile_options(benchmark_opt_3 PUBLIC "-O3")
#Opt 4
add_executable(benchmark_opt_4 benchmark.cpp utils.hpp)
target_include_directories(benchmark_opt_4 PUBLIC "../../code/opt_4")
target_link_libraries(benchmark_opt_4 label_propagation_opt4)
target_compile_options(benchmark_opt_4 PUBLIC "-O3")

