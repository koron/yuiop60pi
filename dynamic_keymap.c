#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"

#include "config.h"
#include "keymap.h"

#ifndef DYNAMIC_KEYMAP_LAYER_COUNT
# define DYNAMIC_KEYMAP_LAYER_COUNT     4
#endif

// FIXME: marshal and unmarshal
static keycode_t dynamic_keymaps[DYNAMIC_KEYMAP_LAYER_COUNT][ROW_NUM][COL_NUM]; 

void dynamic_keymap_reset(void) {
    memset(&dynamic_keymaps, 0, sizeof(dynamic_keymaps));
    memcpy(&dynamic_keymaps, keymaps, MIN(sizeof(dynamic_keymaps), sizeof(keycode_t) * KEYMAP_LAYER_MAX * ROW_NUM * COL_NUM));
}

uint8_t dynamic_keymap_get_layer_count(void) {
    return DYNAMIC_KEYMAP_LAYER_COUNT;
}

uint16_t dynamic_keymap_get_keycode(uint8_t layer, uint8_t row, uint8_t col) {
    printf("dynamic_keymap_get_keycode: layer=%d row=%d col=%d\n", layer, row, col);
    if (layer >= DYNAMIC_KEYMAP_LAYER_COUNT || row >= ROW_NUM || col >= COL_NUM) {
        return KC_NO;
    }
    return dynamic_keymaps[layer][row][col];
}

void dynamic_keymap_set_keycode(uint8_t layer, uint8_t row, uint8_t col, uint16_t keycode) {
    printf("dynamic_keymap_set_keycode: layer=%d row=%d col=%d keycode=%04x\n", layer, row, col, keycode);
    if (layer >= DYNAMIC_KEYMAP_LAYER_COUNT || row >= ROW_NUM || col >= COL_NUM) {
        return;
    }
    dynamic_keymaps[layer][row][col] = keycode;
}

void dynamic_keymap_get_buffer(uint16_t offset, uint16_t size, uint8_t *data) {
    printf("dynamic_keymap_get_buffer: %04x %d\n", offset, size);
    // TODO: implement me.
}

void dynamic_keymap_set_buffer(uint16_t offset, uint16_t size, uint8_t *data) {
    printf("dynamic_keymap_set_buffer: %04x %d\n", offset, size);
    // TODO: implement me.
}

