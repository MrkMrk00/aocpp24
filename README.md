# Advent of C++

An attempt to complete (at least some) challenges of [AoC 2024](https://adventofcode.com/2024) in C++.

The code can be run with the `run.sh` script, but it'll probably only work on Linux (uses gnu coreutils).

## Usage

Put your puzzle input into a folder _input_ under the same name as the solution with the _.txt_ file extension. (Day 8 -> src/08.cpp, input/08.txt)

Running first part of a solution
```{bash}
./run.sh 01
```


Running second part of a solution
```{bash}
./run.sh 01b
```


Using the example data
```{bash}
./run.sh 01b -DTEST
```
<small><code>./run.sh</code> will pass all of the arguments (except the name of the solution) to the C++ compiler.</small>


### Env variables

```{bash}
COMPILER=g++ BIN_PATH=$(realpath ./bin) VERBOSE_TIME=1 ./run.sh 01b -DTEST
```

