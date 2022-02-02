#include <stdio.h>

#include "pico/stdlib.h"

#include "config.h"
#include "keycodes.h"
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

static keycode_t get_keycode(int layer, uint nrow, uint ncol) {
    if (layer < 0 || layer > KEYMAP_LAYER_MAX || keymaps[layer] == 0) {
        return KC_NO;
    }
    return keymaps[layer][nrow][ncol];
}

static bool is_hid_keycode(keycode_t kc) {
    return kc >= KC_A && kc <= 0xff;
}

static bool is_kcx(keycode_t kc, keycode_t constcode, keycode_t mask, uint16_t *value) {
    if ((kc & (~mask)) != constcode) {
        return false;
    }
    if (value != NULL) {
        *value = kc & mask;
    }
    return true;
}

static bool process_layer_action(keycode_t kc, bool on) {
    uint16_t layer = 0;
    // turn on layer when pressed. (TO)
    if (is_kcx(kc, KCX_TO, 0x1f, &layer)) {
        if (on) {
            layer_state = 0;
            layer_set_enable((int)layer);
        }
        return true;
    }
    // momentary turn layer on. (MO)
    if (is_kcx(kc, KCX_MOMENTARY, 0x1f, &layer)) {
        layer_set((int)layer, on);
        return true;
    }
    // toggle layer on/off. (TG)
    if (is_kcx(kc, KCX_TOGGLE_LAYER, 0x1f, &layer)) {
        if (on) {
            layer_toggle((int)layer);
        }
        return true;
    }
    // TODO: implement other layer operations.
    return false;
}

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
