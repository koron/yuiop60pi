#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/time.h"

#include "config.h"
#include "kbd.h"
#include "via.h"

static uint32_t get_layout_options(void) {
    // FIXME: implement me.
    return 0;
}

void set_layout_options(uint32_t value) {
    // FIXME: implement me.
}

static bool get_keyboard_value(uint8_t *cmd, uint8_t *data) {
    switch (data[0]) {
        case via_kbvid_uptime:
            {
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
            // FIXME: fetch matrix state in 28 bytes (224 bits).
            break;

        default:
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
            return false;
    }
    return true;
}

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
            // TODO: implement me.
            break;

        case via_id_dynamic_keymap_set_keycode:
            // TODO: implement me.
            break;

        case via_id_dynamic_keymap_reset:
            // TODO: implement me.
            break;

        case via_id_lighting_set_value:
            // TODO: implement me.
            break;

        case via_id_lighting_get_value:
            // TODO: implement me.
            break;

        case via_id_lighting_save:
            // TODO: implement me.
            break;

        case via_id_eeprom_reset:
            // TODO: implement me.
            break;

        case via_id_bootloader_jump:
            // TODO: implement me.
            break;

        case via_id_dynamic_keymap_macro_get_count:
            // TODO: implement me.
            break;

        case via_id_dynamic_keymap_macro_get_buffer_size:
            // TODO: implement me.
            break;

        case via_id_dynamic_keymap_macro_get_buffer:
            // TODO: implement me.
            break;

        case via_id_dynamic_keymap_macro_set_buffer:
            // TODO: implement me.
            break;

        case via_id_dynamic_keymap_macro_reset:
            // TODO: implement me.
            break;

        case via_id_dynamic_keymap_get_layer_count:
            // TODO: implement me.
            break;

        case via_id_dynamic_keymap_get_buffer:
            // TODO: implement me.
            break;

        case via_id_dynamic_keymap_set_buffer:
            // TODO: implement me.
            break;

        default:
            *cmd = via_id_unhandled;
            break;
    }
    // TODO: dump unhandled command.
    return true;
}
