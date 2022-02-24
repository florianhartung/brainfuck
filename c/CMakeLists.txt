cmake_minimum_required(VERSION 3.21)
project(Brainfuck C)

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror=implicit-function-declaration -Werror=incompatible-pointer-types -Werror=return-type")

add_executable(Brainfuck src/main.c src/brainfuck_interpreter.h src/brainfuck_interpreter.c src/util.h src/util.c)