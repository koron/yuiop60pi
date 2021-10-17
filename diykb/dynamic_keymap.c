#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"

#include "config.h"
#include "nvm.h"
#include "keymap.h"
#include "dynamic_keymap.h"

// debug log switches, 1 to enable.
#define DEBUG_RESET         0
#define DEBUG_GET_KEYCODE   0
#define DEBUG_GET_BUFFER    0

static void load_default(void) {
    memset(&nvm.keymaps, 0, sizeof(nvm.keymaps));
    const size_t sizeof_keymaps = sizeof(keycode_t) * KEYMAP_LAYER_MAX * ROW_NUM * COL_NUM;
    memcpy(&nvm.keymaps, keymaps, MIN(sizeof(nvm.keymaps), sizeof_keymaps));
}

void dynamic_keymap_init(void) {
    // load defualt keymap when nvm has empty keymaps.
    bool is_empty = true;
    for (int i = 0; i < DYNAMIC_KEYMAP_LAYER_COUNT; i++) {
        for (int j = 0; j < ROW_NUM; j++) {
            for (int k = 0; k < COL_NUM; k++) {
                if (nvm.keymaps[i][j][k] != KC_NO) {
                    is_empty = false;
                    break;
                }
            }
        }
    }
    if (is_empty) {
        printf("no keymaps, load default\n");
        load_default();
    }
}

void dynamic_keymap_reset(void) {
#if DEBUG_RESET
    printf("dynamic_keymap_reset\n");
#endif
    load_default();
    nvm_set_modified();
#if DEBUG_RESET
    for (int i = 0; i < DYNAMIC_KEYMAP_LAYER_COUNT; i++) {
        printf("layer #%d\n", i);
        for (int j = 0; j < ROW_NUM; j++) {
            printf("   ");
            for (int k = 0; k < COL_NUM; k++) {
                printf(" %04x", nvm.keymaps[i][j][k]);
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
    return nvm.keymaps[layer][row][col];
}

void dynamic_keymap_set_keycode(uint8_t layer, uint8_t row, uint8_t col, uint16_t keycode) {
    printf("dynamic_keymap_set_keycode: layer=%d row=%d col=%d keycode=%04x\n", layer, row, col, keycode);
    if (layer >= DYNAMIC_KEYMAP_LAYER_COUNT || row >= ROW_NUM || col >= COL_NUM) {
        return;
    }
    nvm.keymaps[layer][row][col] = keycode;
    nvm_set_modified();
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
        uint16_t kc = ((keycode_t *)nvm.keymaps)[x];
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
        uint16_t kc = ((keycode_t *)nvm.keymaps)[x];
        if (y == 0) {
            kc = (kc & 0xff00) | data[i];
        } else {
            kc = (kc & 0x00ff) | ((uint16_t)data[i] << 8);
        }
        ((keycode_t *)nvm.keymaps)[x] = kc;
    }
    nvm_set_modified();
}
