#!/bin/sh

flags=(
    "-std=c89"
    "-pedantic"
    "-I../.."
    "-Wall"
    "-Wno-long-long"
    "-Wno-unicode"
    "-Wno-unused-function"
)
debug="-DBUILD_DEBUG -g -fsanitize=address ${flags[@]}"
release="-DBUILD_RELEASE -O3 ${flags[@]}"

mkdir -p build
cd build

clang $debug ../test.c -o test

cd ..
