#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/time.h"

#include "config.h"
#include "nvm.h"
#include "matrix.h"
#include "kbd.h"
#include "lighting.h"
#include "dynamic_keymap.h"
#include "via.h"

//////////////////////////////////////////////////////////////////////////////
// layout

static uint32_t get_layout_options(void) {
    return nvm.layout_options;
}

static void set_layout_options(uint32_t value) {
    nvm.layout_options = value;
    nvm_set_modified();
}

//////////////////////////////////////////////////////////////////////////////
// lighting

static void lighting_set_value(uint8_t *cmd, uint8_t *data, uint16_t len) {
    printf("VIA: lighting_set_value: %02x %02x\n", *cmd, data[0]);
    switch (data[0]) {
        case via_lightid_backlight_brightness:
            backlight_set_brightness(data[1], false);
            break;
        case via_lightid_backlight_effect:
            backlight_set_effect(data[1], false);
            break;
        case via_lightid_rgblight_brightness:
            {
                rgblight_state_t s;
                rgblight_get_state(&s);
                s.val = data[1];
                rgblight_set_state(&s, false);
            }
            break;
        case via_lightid_rgblight_effect:
            {
                rgblight_state_t s;
                rgblight_get_state(&s);
                s.mode = data[1];
                rgblight_set_state(&s, false);
            }
            break;
        case via_lightid_rgblight_effect_speed:
            {
                rgblight_state_t s;
                rgblight_get_state(&s);
                s.speed = data[1];
                rgblight_set_state(&s, false);
            }
            break;
        case via_lightid_rgblight_color:
            {
                rgblight_state_t s;
                rgblight_get_state(&s);
                s.hue = data[1];
                s.sat = data[2];
                rgblight_set_state(&s, false);
            }
            break;
        default:
            *cmd = via_id_unhandled;
            break;
    }
}

static void lighting_get_value(uint8_t *cmd, uint8_t *data, uint16_t len) {
    printf("VIA: lighting_get_value: %02x %02x\n", *cmd, data[0]);
    switch (data[0]) {
        case via_lightid_backlight_brightness:
            data[1] = backlight_get_brightness();
            break;
        case via_lightid_backlight_effect:
            data[1] = backlight_get_effect();
            break;
        case via_lightid_rgblight_brightness:
            {
                rgblight_state_t s;
                rgblight_get_state(&s);
                data[1] = s.val;
            }
            break;
        case via_lightid_rgblight_effect:
            {
                rgblight_state_t s;
                rgblight_get_state(&s);
                data[1] = s.mode;
            }
            break;
        case via_lightid_rgblight_effect_speed:
            {
                rgblight_state_t s;
                rgblight_get_state(&s);
                data[1] = s.speed;
            }
            break;
        case via_lightid_rgblight_color:
            {
                rgblight_state_t s;
                rgblight_get_state(&s);
                data[1] = s.hue;
                data[2] = s.sat;
            }
            break;
        default:
            *cmd = via_id_unhandled;
            break;
    }
}

static void lighting_save(void) {
    printf("VIA: lighting_save\n");
    backlight_save();
    rgblight_save();
}

//////////////////////////////////////////////////////////////////////////////
// system

static void eeprom_reset(void) {
    // not used in Remap and QMK
    printf("VIA: eeprom_reset\n");
}

static void bootloader_jump(void) {
    // not used in Remap and QMK
    printf("VIA: bootloader_jump\n");
}

//////////////////////////////////////////////////////////////////////////////
// macro

static uint8_t dynamic_keymap_macro_get_count(void) {
    //printf("VIA: dynamic_keymap_macro_get_count\n");
    return DYNAMIC_KEYMAP_MACRO_COUNT;
}

static uint16_t dynamic_keymap_macro_get_buffer_size(void) {
    //printf("VIA: dynamic_keymap_macro_get_buffer_size\n");
    return sizeof(nvm.macro);
}

static void dynamic_keymap_macro_get_buffer(uint16_t offset, uint16_t size, uint8_t *data) {
    //printf("VIA: dynamic_keymap_macro_get_buffer: %04x %d\n", offset, size);
    if (offset < sizeof(nvm.macro) && size > 0) {
        memcpy(data, &(nvm.macro[offset]), MIN(size, sizeof(nvm.macro) - offset));
    }
}

static void dynamic_keymap_macro_set_buffer(uint16_t offset, uint16_t size, uint8_t *data) {
    //printf("VIA: dynamic_keymap_macro_set_buffer: %04x %d\n", offset, size);
    if (offset < sizeof(nvm.macro) && size > 0) {
        memcpy(&(nvm.macro[offset]), data, MIN(size, sizeof(nvm.macro) - offset));
        nvm_set_modified();
    }
}

static void dynamic_keymap_macro_reset(void) {
    //printf("VIA: dynamic_keymap_macro_reset\n");
    memset(nvm.macro, 0, sizeof(nvm.macro));
    nvm_set_modified();
}

//////////////////////////////////////////////////////////////////////////////
// keyboard value

static bool get_keyboard_value(uint8_t *cmd, uint8_t *data) {
    switch (data[0]) {
        case via_kbvid_uptime:
            {
                printf("VIA: get_keyboard_value(uptime)\n");
                uint32_t now = us_to_ms(time_us_64());
                data[1] = (now >> 24) & 0xff;
                data[2] = (now >> 16) & 0xff;
                data[3] = (now >>  8) & 0xff;
                data[4] = (now >>  0) & 0xff;
            }
            break;

        case via_kbvid_layout_options:
            {
                uint32_t opt = get_layout_options();
                data[1] = (opt >> 24) & 0xff;
                data[2] = (opt >> 16) & 0xff;
                data[3] = (opt >>  8) & 0xff;
                data[4] = (opt >>  0) & 0xff;
            }
            break;

        case via_kbvid_matrix_state:
            matrix_get_state(&data[1], 28);
            break;

        default:
            printf("VIA: get_keyboard_value(unknown): %02x\n", *cmd);
            return false;
    }
    return true;
}

static bool set_keyboard_value(uint8_t *cmd, uint8_t *data) {
    switch (data[0]) {
        case via_kbvid_layout_options:
            {
                uint32_t opt = ((uint32_t)data[1] << 24)
                    | ((uint32_t)data[2] << 16)
                    | ((uint32_t)data[3] <<  8)
                    | ((uint32_t)data[4] <<  0);
                set_layout_options(opt);
            }
            break;

        default:
            printf("VIA: set_keyboard_value: %02x\n", *cmd);
            return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////
// dispatch VIA commands

bool kbd_handle_via_command(uint8_t *cmd, uint8_t *data, uint16_t len) {
    switch (*cmd) {
        case via_id_get_protocol_version:
            data[0] = VIA_PROTOCOL_VERSION >> 8;
            data[1] = VIA_PROTOCOL_VERSION & 0xff;
            break;

        case via_id_get_keyboard_value:
            return get_keyboard_value(cmd, data);

        case via_id_set_keyboard_value:
            return set_keyboard_value(cmd, data);

        case via_id_dynamic_keymap_get_keycode:
            {
                uint16_t kc = dynamic_keymap_get_keycode(data[0], data[1], data[2]);
                data[3] = (kc >> 8) & 0xff;
                data[4] = (kc >> 0) & 0xff;
            }
            break;

        case via_id_dynamic_keymap_set_keycode:
            {
                uint16_t kc = ((uint16_t)data[3] << 8) | data[4];
                dynamic_keymap_set_keycode(data[0], data[1], data[2], kc);
            }
            break;

        case via_id_dynamic_keymap_reset:
            dynamic_keymap_reset();
            break;

        case via_id_lighting_set_value:
            lighting_set_value(cmd, data, len);
            break;

        case via_id_lighting_get_value:
            lighting_get_value(cmd, data, len);
            break;

        case via_id_lighting_save:
            lighting_save();
            break;

        case via_id_eeprom_reset:
            eeprom_reset();
            break;

        case via_id_bootloader_jump:
            bootloader_jump();
            break;

        case via_id_dynamic_keymap_macro_get_count:
            data[0] = dynamic_keymap_macro_get_count();
            break;

        case via_id_dynamic_keymap_macro_get_buffer_size:
            {
                uint16_t sz = dynamic_keymap_macro_get_buffer_size();
                data[0] = (sz >> 8) & 0xff;
                data[1] = (sz >> 0) & 0xff;
            }
            break;

        case via_id_dynamic_keymap_macro_get_buffer:
            {
                uint16_t offset = ((uint16_t)data[0] << 8) | data[1];
                uint16_t size = MIN(data[2], 28);
                dynamic_keymap_macro_get_buffer(offset, size, &data[3]);
            }
            break;

        case via_id_dynamic_keymap_macro_set_buffer:
            {
                uint16_t offset = ((uint16_t)data[0] << 8) | data[1];
                uint16_t size = MIN(data[2], 28);
                dynamic_keymap_macro_set_buffer(offset, size, &data[3]);
            }
            break;

        case via_id_dynamic_keymap_macro_reset:
            dynamic_keymap_macro_reset();
            break;

        case via_id_dynamic_keymap_get_layer_count:
            data[0] = dynamic_keymap_get_layer_count();
            break;

        case via_id_dynamic_keymap_get_buffer:
            {
                uint16_t offset = ((uint16_t)data[0] << 8) | data[1];
                uint16_t size = MIN(data[2], 28);
                dynamic_keymap_get_buffer(offset, size, &data[3]);
            }
            break;

        case via_id_dynamic_keymap_set_buffer:
            {
                uint16_t offset = ((uint16_t)data[0] << 8) | data[1];
                uint16_t size = MIN(data[2], 28);
                dynamic_keymap_set_buffer(offset, size, &data[3]);
            }
            break;

        default:
            *cmd = via_id_unhandled;
            break;
    }
    return true;
}
