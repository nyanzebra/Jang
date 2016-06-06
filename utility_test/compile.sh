# !/bin/bash

clang++ -stdlib=libc++ -fmodules -g -std=c++1z -o utility_test src/main.cpp 2> compiler_output.txt
less compiler_output.txt
