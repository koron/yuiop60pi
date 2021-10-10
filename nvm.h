#pragma once

#include "config.h"
#include "keycodes.h"
#include "dynamic_keymap.h"
#include "lighting.h"

typedef struct {
    // management headers
    uint8_t flags;
    uint8_t rev;

    // for via
    uint32_t layout_options;

    // for dynamic_keymap
    keycode_t keymaps[DYNAMIC_KEYMAP_LAYER_COUNT][ROW_NUM][COL_NUM];

    uint8_t macro[DYNAMIC_KEYMAP_MACRO_BUFFERSIZE];

    // for lightning
    rgblight_state_t rgblight;
} nvm_block_t;

extern nvm_block_t nvm;

#ifdef __cplusplus
extern "C" {
#endif

void nvm_init();

void nvm_task(uint64_t now);

void nvm_set_modified(void);

#ifdef __cplusplus
}
#endif
