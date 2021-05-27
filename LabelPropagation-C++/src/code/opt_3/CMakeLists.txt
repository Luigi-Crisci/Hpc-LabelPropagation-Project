add_library(label_propagation_opt3)


target_sources(label_propagation_opt3
  PRIVATE
  label_propagation.cpp
  utils.cpp
  PUBLIC
  headers/label_propagation.h
  headers/mtrnd.h
)
target_link_libraries(label_propagation_opt3 
                    PRIVATE OpenMP::OpenMP_CXX)


target_include_directories(label_propagation_opt3
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

set(CMAKE_CXX_FLAGS "-O3")

set(CMAKE_CXX_FLAGS "-DDEBUG")