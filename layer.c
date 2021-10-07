#include "pico/stdlib.h"
#include "tusb.h"

#include "config.h"
#include "keycodes.h"
#include "dynamic_keymap.h"

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
    layer_state |= ~(1 << (layer - 1));
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

static bool is_hid_keycode(keycode_t kc) {
    return kc >= KC_A && kc <= 0xff;
}

static bool is_kcx_qk(keycode_t kc, keycode_t constcode, keycode_t mask, uint16_t *value) {
    if ((kc & (~mask)) != constcode) {
        return false;
    }
    if (value != NULL) {
        *value = kc & mask;
    }
    return true;
}

uint8_t layer_get_code(uint ncol, uint nrow, bool on) {
    keycode_t kc = 0;
    for (uint8_t i = LAYER_MAXNUM; i >= 0; i--) {
        if (!layer_is_enabled(i)) {
            continue;
        }
        kc = dynamic_keymap_get_keycode(i, nrow, ncol);
        // continue when kc is KC_TRANSPARENT.
        if (kc != KC_TRANSPARENT) {
            break;
        }
    }
    if (kc == KC_NO || kc == KC_TRANSPARENT) {
        // no HID code mapped.
        return 0;
    }
    // send HID code immediately.
    if (is_hid_keycode(kc)) {
        return (uint8_t)kc;
    }

    uint16_t layer = 0;

    // turn on layer when pressed. (TO)
    if (is_kcx_qk(QK_TO, 0x0f, kc, &layer)) {
        if (on) {
            layer_state = 0;
            layer_set_enable((int)layer);
        }
        return 0;
    }
    // momentary turn layer on. (MO)
    if (is_kcx_qk(QK_MOMENTARY, 0xff, kc, &layer)) {
        layer_set((int)layer, on);
        return 0;
    }
    // toggle layer on/off. (TG)
    if (is_kcx_qk(QK_TOGGLE_LAYER, 0xff, kc, &layer)) {
        if (on) {
            layer_toggle((int)layer);
        }
        return 0;
    }

    // TODO: implement other layer operations.

    return 0;
}
