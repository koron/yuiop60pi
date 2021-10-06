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
    via_id_eeprom_reset                         = 0x0a,
    via_id_bootloader_jump                      = 0x0b,
    via_id_dynamic_keymap_macro_get_count       = 0x0c,
    via_id_dynamic_keymap_macro_get_buffer_size = 0x0d,
    via_id_dynamic_keymap_macro_get_buffer      = 0x0e,
    via_id_dynamic_keymap_macro_set_buffer      = 0x0f,
    via_id_dynamic_keymap_macro_reset           = 0x10,
    via_id_dynamic_keymap_get_layer_count       = 0x11,
    via_id_dynamic_keymap_get_buffer            = 0x12,
    via_id_dynamic_keymap_set_buffer            = 0x13,
    via_id_unhandled                            = 0xff,
};

enum {
    via_kbvid_uptime         = 0x01,
    via_kbvid_layout_options = 0x02,
    via_kbvid_matrix_state   = 0x03,
};

enum {
    via_lightid_backlight_brightness   = 0x09,
    via_lightid_backlight_effect       = 0x0a,

    via_lightid_rgblight_brightness    = 0x80,
    via_lightid_rgblight_effect        = 0x81,
    via_lightid_rgblight_effect_speed  = 0x82,
    via_lightid_rgblight_color         = 0x83,
};

enum {
    via_keycode_fn_mo13 = 0x5f10;
    via_keycode_fn_mo23 = 0x5f11;
    via_keycode_macro00 = 0x5f12;
    via_keycode_macro01 = 0x5f13;
    via_keycode_macro02 = 0x5f14;
    via_keycode_macro03 = 0x5f15;
    via_keycode_macro04 = 0x5f16;
    via_keycode_macro05 = 0x5f17;
    via_keycode_macro06 = 0x5f18;
    via_keycode_macro07 = 0x5f19;
    via_keycode_macro08 = 0x5f1a;
    via_keycode_macro09 = 0x5f1b;
    via_keycode_macro10 = 0x5f1c;
    via_keycode_macro11 = 0x5f1d;
    via_keycode_macro12 = 0x5f1e;
    via_keycode_macro13 = 0x5f1f;
    via_keycode_macro14 = 0x5f20;
    via_keycode_macro15 = 0x5f21;

    via_keycode_user00 = 0x5f80;
    via_keycode_user01 = 0x5f81;
    via_keycode_user02 = 0x5f82;
    via_keycode_user03 = 0x5f83;
    via_keycode_user04 = 0x5f84;
    via_keycode_user05 = 0x5f85;
    via_keycode_user06 = 0x5f86;
    via_keycode_user07 = 0x5f87;
    via_keycode_user08 = 0x5f88;
    via_keycode_user09 = 0x5f89;
    via_keycode_user10 = 0x5f8a;
    via_keycode_user11 = 0x5f8b;
    via_keycode_user12 = 0x5f8c;
    via_keycode_user13 = 0x5f8d;
    via_keycode_user14 = 0x5f8e;
    via_keycode_user15 = 0x5f8f;
};
