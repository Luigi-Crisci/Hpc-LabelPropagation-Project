#!/bin/bash
mkdir build &> /dev/null
cd build
cmake -D CMAKE_CXX_COMPILER=clang++-11 -D CMAKE_BUILD_TYPE=Debug ..
make -j 8 all
