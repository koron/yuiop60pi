#include <stdio.h>

#include "pico/stdlib.h"
#include "tusb.h"

#include "config.h"
#include "keycodes.h"
#include "dynamic_keymap.h"
#include "backlight.h"
#include "rgb.h"

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

static bool process_layer_action(keycode_t kc, bool on) {
    uint16_t layer = 0;
    // turn on layer when pressed. (TO)
    if (is_kcx_qk(kc, QK_TO, 0x0f, &layer)) {
        if (on) {
            layer_state = 0;
            layer_set_enable((int)layer);
        }
        return true;
    }
    // momentary turn layer on. (MO)
    if (is_kcx_qk(kc, QK_MOMENTARY, 0xff, &layer)) {
        layer_set((int)layer, on);
        return true;
    }
    // toggle layer on/off. (TG)
    if (is_kcx_qk(kc, QK_TOGGLE_LAYER, 0xff, &layer)) {
        if (on) {
            layer_toggle((int)layer);
        }
        return true;
    }
    // TODO: implement other layer operations.
    return false;
}

static bool process_backlight_action(keycode_t kc, bool on) {
    if (!on) {
        return false;
    }
    switch (kc) {
        case BL_ON:
            backlight_act_on();
            break;
        case BL_OFF:
            backlight_act_off();
            break;
        case BL_DEC:
            backlight_act_dec();
            break;
        case BL_INC:
            backlight_act_inc();
            break;
        case BL_TOGG:
            backlight_act_togg();
            break;
        case BL_STEP:
            backlight_act_step();
            break;
        case BL_BRTG:
            backlight_act_brtg();
            break;
        default:
            return false;
    }
    return true;
}

static bool process_rgb_action(keycode_t kc, bool on) {
    if (!on) {
        return false;
    }
    switch (kc) {
        case RGB_TOG:
            rgb_act_tog();
            break;
        case RGB_MODE_FORWARD:
            rgb_act_mode_forward();
            break;
        case RGB_MODE_REVERSE:
            rgb_act_mode_reverse();
            break;
        case RGB_HUI:
            rgb_act_hui();
            break;
        case RGB_HUD:
            rgb_act_hud();
            break;
        case RGB_SAI:
            rgb_act_sai();
            break;
        case RGB_SAD:
            rgb_act_sad();
            break;
        case RGB_VAI:
            rgb_act_vai();
            break;
        case RGB_VAD:
            rgb_act_vad();
            break;
        case RGB_SPI:
            rgb_act_spi();
            break;
        case RGB_SPD:
            rgb_act_spd();
            break;
        case RGB_MODE_PLAIN:
            rgb_act_mode_plain();
            break;
        case RGB_MODE_BREATHE:
            rgb_act_mode_breathe();
            break;
        case RGB_MODE_RAINBOW:
            rgb_act_mode_rainbow();
            break;
        case RGB_MODE_SWIRL:
            rgb_act_mode_swirl();
            break;
        case RGB_MODE_SNAKE:
            rgb_act_mode_snake();
            break;
        case RGB_MODE_KNIGHT:
            rgb_act_mode_knight();
            break;
        case RGB_MODE_XMAS:
            rgb_act_mode_xmas();
            break;
        case RGB_MODE_GRADIENT:
            rgb_act_mode_gradient();
            break;
        case RGB_MODE_RGBTEST:
            rgb_act_mode_rgbtest();
            break;
        default:
            return false;
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
    if (process_backlight_action(kc, on)) {
        return 0;
    }
    if (process_rgb_action(kc, on)) {
        return 0;
    }
    // FIXME: add more actions.
    // no actions mapped.
    return 0;
}
