cmake_minimum_required(VERSION 3.21)
project(Brainfuck C)

set(CMAKE_C_FLAGS "-Wall -Wextra")

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /we4013 /we4133 /we4716")
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=implicit-function-declaration -Werror=incompatible-pointer-types -Werror=return-type")
endif()

add_executable(Brainfuck src/main.c src/brainfuck_interpreter.h src/brainfuck_interpreter.c src/util.h)
