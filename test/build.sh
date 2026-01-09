#!/bin/sh

ignore_warnings=(
    "-Wno-comment"
    "-Wno-long-long"
    "-Wno-pointer-sign"
    "-Wno-return-type"
    "-Wno-unused"
)
flags="-std=c89 -pedantic -I.. -Wall ${ignore_warnings[@]}"
debug_flags="-DBUILD_DEBUG -g -fsanitize=address $flags"
release_flags="-DBUILD_RELEASE -O3 $flags"

mkdir -p ../build
cd ../build

clang $debug_flags ../test/test.c -o test

cd ..
