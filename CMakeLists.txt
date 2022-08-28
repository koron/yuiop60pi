cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(yuiop60pi C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pimoroni_pga2040)

pico_sdk_init()

add_subdirectory(diykb)
add_subdirectory(yuiop60pi)

# vim:set ts=8 sts=4 sw=4 tw=0 et:
