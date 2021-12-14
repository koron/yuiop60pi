#pragma once

#include "config.h"

#ifndef DYNAMIC_KEYMAP_LAYER_COUNT
# define DYNAMIC_KEYMAP_LAYER_COUNT     4
#endif

#ifndef DYNAMIC_KEYMAP_MACRO_COUNT
# define DYNAMIC_KEYMAP_MACRO_COUNT     4
#endif

#ifndef DYNAMIC_KEYMAP_MACRO_BUFFERSIZE
# define DYNAMIC_KEYMAP_MACRO_BUFFERSIZE    1024
#endif

#ifdef __cplusplus
extern "C" {
#endif

void dynamic_keymap_init(void);

void dynamic_keymap_reset(void);

uint8_t dynamic_keymap_get_layer_count(void);

uint16_t dynamic_keymap_get_keycode(uint8_t layer, uint8_t row, uint8_t col);

void dynamic_keymap_set_keycode(uint8_t layer, uint8_t row, uint8_t col, uint16_t keycode);

void dynamic_keymap_get_buffer(uint16_t offset, uint16_t size, uint8_t *data);

void dynamic_keymap_set_buffer(uint16_t offset, uint16_t size, uint8_t *data);

#ifdef __cplusplus
}
#endif
