# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(yuiop60pi C CXX ASM)

# Initialise the Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(yuiop60pi
	main.c
	layout.c
	keymap.c
	diykb/usb_descriptors.c
	diykb/matrix.c
	diykb/keycode.c
	diykb/layer.c
	diykb/action.c
	diykb/kbd.c
	diykb/ledarray.c
	# WIP
	diykb/backlight.c
	)

# Link Time Optimization is not supported yet.
#set_property(TARGET yuiop60pi PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

pico_set_program_name(yuiop60pi "yuiop60pi")
pico_set_program_version(yuiop60pi "0.2")

pico_enable_stdio_uart(yuiop60pi 1)
pico_enable_stdio_usb(yuiop60pi 0)

target_include_directories(yuiop60pi PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_LIST_DIR}/diykb
	)

target_link_libraries(yuiop60pi
	pico_stdlib
	pico_unique_id
	pico_time
	pico_multicore
	hardware_pio
	hardware_dma
	tinyusb_device
	tinyusb_board
	)

pico_add_extra_outputs(yuiop60pi)
