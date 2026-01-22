cmake_minimum_required(VERSION 3.20.0)
project(main)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB sources "src/main.c")
add_executable(main ${sources})

include_directories(main PRIVATE include)