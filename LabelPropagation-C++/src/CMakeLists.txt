add_executable(label_propagation)

target_link_libraries(label_propagation PRIVATE OpenMP::OpenMP_CXX)

target_sources(label_propagation
  PUBLIC
	label_propagation.cpp
)

target_include_directories(label_propagation
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )


