add_library(diykb INTERFACE)

target_include_directories(diykb INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

target_sources(diykb INTERFACE
	action.c
	light.c
	kbd.c
	keycode.c
	layer.c
	ledarray.c
	matrix.c
	usb_descriptors.c
	)

target_link_libraries(diykb INTERFACE
	pico_stdlib
	pico_unique_id
	hardware_dma
	hardware_pio
	hardware_timer
	tinyusb_device
	tinyusb_board
	)
