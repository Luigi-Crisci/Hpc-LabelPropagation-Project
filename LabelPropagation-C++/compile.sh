#!/bin/bash
mkdir build &> /dev/null
cd build
cmake -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Release ..
make -j $(nproc) all
