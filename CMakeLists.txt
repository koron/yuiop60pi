# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(olsk60 C CXX ASM)

# Initialise the Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(olsk60
	main.c
	keymap.c
	diykb/usb_descriptors.c
	diykb/matrix_duplex.c
	diykb/keycode.c
	diykb/layer.c
	diykb/action.c
	diykb/kbd.c
	diykb/ledarray.c
	)

# Link Time Optimization is not supported yet.
#set_property(TARGET olsk60 PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

pico_set_program_name(olsk60 "olsk60")
pico_set_program_version(olsk60 "0.2")

pico_enable_stdio_uart(olsk60 0)
pico_enable_stdio_usb(olsk60 0)

target_include_directories(olsk60 PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_LIST_DIR}/diykb
	)

target_link_libraries(olsk60
	pico_stdlib
	pico_unique_id
	pico_time
	pico_multicore
	hardware_pio
	hardware_dma
	tinyusb_device
	tinyusb_board
	)

pico_add_extra_outputs(olsk60)
