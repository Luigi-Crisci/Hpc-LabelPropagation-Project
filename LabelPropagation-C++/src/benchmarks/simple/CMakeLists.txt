add_executable(simple simple.cpp)

# include_directories("../../code/sequential")
target_link_libraries(simple 
                    PRIVATE OpenMP::OpenMP_CXX)

set(CMAKE_CXX_FLAGS "-O3")
