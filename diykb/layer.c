#include <stdio.h>

#include "pico/stdlib.h"

#include "config.h"
#include "keycode.h"
#include "keymap.h"

#define LAYER_MAXNUM	31

uint32_t layer_state = 0;

bool layer_is_enabled(uint8_t layer) {
    if (layer < 0 || layer > LAYER_MAXNUM) {
        return false;
    }
    if (layer == 0) {
        return true;
    }
    return (layer_state & (1 << (layer - 1))) != 0;
}

void layer_set_enable(int layer) {
    if (layer < 1 || layer > LAYER_MAXNUM) {
        return;
    }
    layer_state |= (1 << (layer - 1));
}

void layer_set_disable(int layer) {
    if (layer < 1 || layer > LAYER_MAXNUM) {
        return;
    }
    layer_state &= ~(1 << (layer - 1));
}

void layer_set(int layer, bool enable) {
    if (enable) {
        layer_set_enable(layer);
    } else {
        layer_set_disable(layer);
    }
}

void layer_toggle(int layer) {
    layer_set(layer, !layer_is_enabled(layer));
}

static keycode_t get_keycode(int layer, uint knum) {
    if (layer < 0 || layer > KEYMAP_LAYER_MAX || keymaps[layer] == 0) {
        return KC_NO;
    }
    return keymaps[layer][knum];
}

keycode_t layer_get_keycode(uint knum) {
    keycode_t kc = 0;
    for (uint8_t i = LAYER_MAXNUM; i >= 0; i--) {
        if (!layer_is_enabled(i)) {
            continue;
        }
        kc = get_keycode(i, knum);
        // continue when kc is KC_TRANSPARENT.
        if (kc != KC_TRANSPARENT) {
            break;
        }
    }
    // no HID code mapped.
    if (kc == KC_TRANSPARENT) {
        kc = KC_NO;
    }
    return kc;
}

#if 0
uint8_t layer_get_code(uint ncol, uint nrow, bool on) {
    keycode_t kc = 0;
    for (uint8_t i = LAYER_MAXNUM; i >= 0; i--) {
        if (!layer_is_enabled(i)) {
            continue;
        }
        kc = get_keycode(i, nrow, ncol);
        // continue when kc is KC_TRANSPARENT.
        if (kc != KC_TRANSPARENT) {
            break;
        }
    }
    // no HID code mapped.
    if (kc == KC_NO || kc == KC_TRANSPARENT) {
        return 0;
    }
    // send HID code immediately.
    if (is_hid_keycode(kc)) {
        return (uint8_t)kc;
    }
    // process various actoins.
    if (process_layer_action(kc, on)) {
        return 0;
    }
    // FIXME: add more actions.
    // no actions mapped.
    return 0;
}
#endif
