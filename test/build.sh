#!/bin/sh

debug_flags="-DBUILD_DEBUG -g -std=c99 -I.. -Wall -Wno-unused -Wno-pointer-sign"
release_flags="-DBUILD_RELEASE -O3 -std=c99 -I.. -Wall -Wno-unused -Wno-pointer-sign"

mkdir -p ../build
cd ../build

clang $debug_flags ../test/test.c -o test

cd ..
