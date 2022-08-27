cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(yuiop60pi C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(diykb)
add_subdirectory(yuiop60pi)
