#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"

#include "config.h"
#include "keymap.h"
#include "dynamic_keymap.h"

#ifndef DYNAMIC_KEYMAP_LAYER_COUNT
# define DYNAMIC_KEYMAP_LAYER_COUNT     4
#endif

// debug log switches, 1 to enable.
#define DEBUG_RESET         0
#define DEBUG_GET_KEYCODE   0
#define DEBUG_GET_BUFFER    0

// FIXME: marshal and unmarshal
static keycode_t dynamic_keymaps[DYNAMIC_KEYMAP_LAYER_COUNT][ROW_NUM][COL_NUM]; 

void dynamic_keymap_init(void) {
    // FIXME: load persited keymaps.
    dynamic_keymap_reset();
}

void dynamic_keymap_reset(void) {
#if DEBUG_RESET
    printf("dynamic_keymap_reset\n");
#endif
    memset(&dynamic_keymaps, 0, sizeof(dynamic_keymaps));
    const size_t sizeof_keymaps = sizeof(keycode_t) * KEYMAP_LAYER_MAX * ROW_NUM * COL_NUM;
    memcpy(&dynamic_keymaps, keymaps, MIN(sizeof(dynamic_keymaps), sizeof_keymaps));
#if DEBUG_RESET
    for (int i = 0; i < DYNAMIC_KEYMAP_LAYER_COUNT; i++) {
        printf("layer #%d\n", i);
        for (int j = 0; j < ROW_NUM; j++) {
            printf("   ");
            for (int k = 0; k < COL_NUM; k++) {
                printf(" %04x", dynamic_keymaps[i][j][k]);
            }
            printf("\n");
        }
    }
#endif
}

uint8_t dynamic_keymap_get_layer_count(void) {
    return DYNAMIC_KEYMAP_LAYER_COUNT;
}

uint16_t dynamic_keymap_get_keycode(uint8_t layer, uint8_t row, uint8_t col) {
#if DEBUG_GET_KEYCODE
    printf("dynamic_keymap_get_keycode: layer=%d row=%d col=%d\n", layer, row, col);
#endif
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
#if DEBUG_GET_BUFFER
    printf("dynamic_keymap_get_buffer: %04x %d\n", offset, size);
    printf("   ");
#endif
    for (uint16_t i = 0; i < size; i++) {
        uint16_t x = offset >> 1;
        uint16_t y = offset & 0x0001;
        offset++;
        uint16_t kc = ((keycode_t *)dynamic_keymaps)[x];
        data[i] = y == 0 ? kc >> 8 : kc & 0xff;
#if DEBUG_GET_BUFFER
        if (y == 0) {
            printf(" %04x", kc);
        }
#endif
    }
#if DEBUG_GET_BUFFER
    printf("\n");
    printf("   ");
    for (uint16_t i = 0; i < size; i++) {
        printf(" %02x", data[i]);
    }
    printf("\n");
#endif
}

void dynamic_keymap_set_buffer(uint16_t offset, uint16_t size, uint8_t *data) {
    printf("dynamic_keymap_set_buffer: %04x %d\n", offset, size);
    for (uint16_t i = 0; i < size; i++) {
        uint16_t x = offset >> 1;
        uint16_t y = offset & 0x0001;
        offset++;
        uint16_t kc = ((keycode_t *)dynamic_keymaps)[x];
        if (y == 0) {
            kc = (kc & 0xff00) | data[i];
        } else {
            kc = (kc & 0x00ff) | ((uint16_t)data[i] << 8);
        }
        ((keycode_t *)dynamic_keymaps)[x] = kc;
    }
}
