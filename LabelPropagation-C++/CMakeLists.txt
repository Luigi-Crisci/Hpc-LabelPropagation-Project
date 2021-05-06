cmake_minimum_required(VERSION 3.0.0)
project(LabelPropagation-C++ VERSION 0.1.0)

find_package(OpenMP REQUIRED)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O0 -Wno-dev")

add_subdirectory(src)
enable_testing()