#!/bin/sh

flags=(
    "-std=c89"
    "-pedantic"
    "-I.."
    "-Wall"
    "-Wno-long-long"
    "-Wno-unused-function"
)
debug="-DBUILD_DEBUG -g -fsanitize=address -fno-omit-frame-pointer ${flags[@]}"
release="-DBUILD_RELEASE -O3 ${flags[@]}"

cd $(realpath $(dirname $0))
mkdir -p ../build
cd ../build

clang $debug ../test/test.c -o test

cd ..
