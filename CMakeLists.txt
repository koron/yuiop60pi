cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pimoroni_pga2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(pico_sdk_import.cmake)

project(yuiop60pi C CXX ASM)

pico_sdk_init()

add_subdirectory(diykb)
add_subdirectory(yuiop60pi)
