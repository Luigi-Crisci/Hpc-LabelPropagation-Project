add_executable(simple simple.cpp)

# include_directories("../../code/sequential")
set(CMAKE_CXX_FLAGS "-fopenmp")

set(CMAKE_CXX_FLAGS "-O3")
