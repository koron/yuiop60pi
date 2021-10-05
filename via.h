#pragma once

#define VIA_PROTOCOL_VERSION 0x0009

enum {
    via_id_get_protocol_version                 = 0x01,
    via_id_get_keyboard_value                   = 0x02,
    via_id_set_keyboard_value                   = 0x03,
    via_id_dynamic_keymap_get_keycode           = 0x04,
    via_id_dynamic_keymap_set_keycode           = 0x05,
    via_id_dynamic_keymap_reset                 = 0x06,
    via_id_lighting_set_value                   = 0x07,
    via_id_lighting_get_value                   = 0x08,
    via_id_lighting_save                        = 0x09,
    via_id_eeprom_reset                         = 0x0A,
    via_id_bootloader_jump                      = 0x0B,
    via_id_dynamic_keymap_macro_get_count       = 0x0C,
    via_id_dynamic_keymap_macro_get_buffer_size = 0x0D,
    via_id_dynamic_keymap_macro_get_buffer      = 0x0E,
    via_id_dynamic_keymap_macro_set_buffer      = 0x0F,
    via_id_dynamic_keymap_macro_reset           = 0x10,
    via_id_dynamic_keymap_get_layer_count       = 0x11,
    via_id_dynamic_keymap_get_buffer            = 0x12,
    via_id_dynamic_keymap_set_buffer            = 0x13,
    via_id_unhandled                            = 0xFF,
};

enum {
    via_kbvid_uptime         = 0x01,
    via_kbvid_layout_options = 0x02,
    via_kbvid_matrix_state   = 0x03,
};
