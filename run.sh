#!/usr/bin/env bash

COMPILER="clang++"
FLAGS="-o2 -Wall -std=c++20"
EXE="/tmp/aocpp24"

$COMPILER $FLAGS "./src/$1.cpp" ${@:2} -o $EXE

time $EXE
