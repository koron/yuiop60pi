add_executable(yuiop60pi layout.c main.c keymap.c)

## Link Time Optimization is not supported yet.
#set_property(TARGET yuiop60pi PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

pico_set_program_name(yuiop60pi "yuiop60pi")
pico_set_program_url(yuiop60pi "https://github.com/koron/yuiop60pi")
pico_set_program_description(yuiop60pi "DIY keyboard with PGA2040 firmware")

pico_enable_stdio_uart(yuiop60pi 1)
pico_enable_stdio_usb(yuiop60pi 0)

target_include_directories(yuiop60pi PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(yuiop60pi
	diykb
	pico_time
	pico_multicore
	pico_binary_info
	tinyusb_device
	tinyusb_board
	)

pico_add_extra_outputs(yuiop60pi)
