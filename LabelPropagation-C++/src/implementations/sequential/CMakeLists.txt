add_library(label_propagation)


target_sources(label_propagation
  PRIVATE
  label_propagation.cpp
  utils.cpp
  PUBLIC
  headers/label_propagation.h
  headers/mtrnd.h
)

target_include_directories(label_propagation
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )


