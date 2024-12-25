#!/usr/bin/env bash

[[ "${COMPILER}" != ""]] || COMPILER="clang++"
FLAGS="-o2 -std=c++23 -Wall -Wextra -Wpedantic -Werror -Wfloat-equal -fdiagnostics-color=always -Wno-sign-compare ${FLAGS}"

if [[ "${BIN_PATH}" == "" ]]; then
    BIN_PATH="/tmp/aocpp24"
fi

if [[ "$1" == "clean" ]]; then
    echo "Cleaning up the binary cache..."
    rm $BIN_PATH/aocpp24-*.out

    exit 0
fi

SOURCE=$1
ARGS=${@:2}
ARGS_EXE=""

# If there is a single source file for both parts of the puzzle.
if [[ $SOURCE == *"b" && ! -f "./src/${SOURCE}.cpp" ]]; then
  SOURCE="${SOURCE:0:2}"
  ARGS_EXE="${ARGS_EXE} -b"
fi

SOURCE_ABS=$(realpath "./src/${SOURCE}.cpp")

# Hash the compiler input -> no need to recompile of nothing changed.
# The source file itself and the compiler used, compiler flags, -D defines, ...
ARGS_SUM=$(echo "${COMPILER}${FLAGS}${ARGS}" | md5sum | cut -d' ' -f1)
EXE="${BIN_PATH}/aocpp24-$(md5sum $SOURCE_ABS | cut -d' ' -f1)-${ARGS_SUM}.out"

[[ -d "$BIN_PATH" ]] || mkdir -p "$BIN_PATH"

if [[ ! -f "$EXE" ]]; then
    echo "Compiling ${SOURCE} with custom args: \"${ARGS}\""
    $COMPILER $FLAGS $SOURCE_ABS $ARGS -o $EXE || exit 1
else
    echo "Using cached executable: \"${EXE}\""
fi

echo "Running ${SOURCE} with args: \"$ARGS_EXE\""
echo "-----------------------------------"

if [[ -f "/usr/bin/time" && "${VERBOSE_TIME}" == "1" ]]; then
    /usr/bin/time --verbose $EXE $ARGS_EXE 2>&1
else
    time $EXE $ARGS_EXE
fi

