#include <stdio.h>

#include "pico/stdlib.h"
#include "tusb.h"

#include "config.h"
#include "usb_descriptors.h"
#include "keycodes.h"
#include "kbd.h"

static bool kbd_changed = false;
static uint8_t kbd_mod = 0;
static uint8_t kbd_code[6] = {0};

static uint8_t kbd_code2mod(uint8_t code) {
    uint8_t mod = 0;
    switch (code) {
        case KC_LCTRL:  // HID_KEY_CONTROL_LEFT
            mod = KEYBOARD_MODIFIER_LEFTCTRL;
            break;
        case KC_LSHIFT: // HID_KEY_SHIFT_LEFT
            mod = KEYBOARD_MODIFIER_LEFTSHIFT;
            break;
        case KC_LALT:   // HID_KEY_ALT_LEFT
            mod = KEYBOARD_MODIFIER_LEFTALT;
            break;
        case KC_LGUI:   // HID_KEY_GUI_LEFT
            mod = KEYBOARD_MODIFIER_LEFTGUI;
            break;
        case KC_RCTRL:  // HID_KEY_CONTROL_RIGHT
            mod = KEYBOARD_MODIFIER_RIGHTCTRL;
            break;
        case KC_RSHIFT: // HID_KEY_SHIFT_RIGHT
            mod = KEYBOARD_MODIFIER_RIGHTSHIFT;
            break;
        case KC_RALT:   // HID_KEY_ALT_RIGHT
            mod = KEYBOARD_MODIFIER_RIGHTALT;
            break;
        case KC_RGUI:   // HID_KEY_GUI_RIGHT
            mod = KEYBOARD_MODIFIER_RIGHTGUI;
            break;
    }
    return mod;
}

// kbd_report_code composes keyboard report which will be send.
void kbd_report_code(uint8_t code, bool on) {
    //printf("kbd_report_code: %02X %s\n", code, on ? "ON" : "OFF");
    // update modifier states.
    uint8_t mod = kbd_code2mod(code);
    if (mod != 0) {
        if (on) {
            kbd_mod |= mod;
        } else {
            kbd_mod &= ~mod;
        }
        kbd_changed |= true;
        return;
    }
    // update kbd_code.
    int found = -1, vacant = -1;
    for (int i = 0; i < count_of(kbd_code); i++) {
        if (vacant < 0 && kbd_code[i] == 0) {
            vacant = i;
        }
        if (found < 0 && kbd_code[i] == code) {
            found = i;
        }
    }
    // when key up.
    if (!on) {
        if (found >= 0) {
            kbd_code[found] = 0;
            kbd_changed |= true;
        }
        return;
    }
    // when key down.
    if (found >= 0 || vacant < 0) {
        return;
    }
    kbd_code[vacant] = code;
    kbd_changed |= true;
}

void kbd_task(uint64_t now) {
    if (!kbd_changed) {
        return;
    }
    // clean up kbd_code. remove intermediate zeros, padding non-zero
    // codes to left.
    bool aligned = false;
    uint8_t tmp[6] = {0};
    for (int i = 0, j = 0; i < count_of(kbd_code); i++) {
        if (kbd_code[i] != 0) {
            tmp[j] = kbd_code[i];
            if (j != i) {
                aligned |= true;
            }
            j++;
        }
    }
    if (aligned) {
        memcpy(kbd_code, tmp, sizeof(kbd_code));
    }
    // send keyboard report with boot protocol.
    //printf("hid_task: keyboard: %02X (%02X %02X %02X %02X %02X %02X)\n", kbd_mod, kbd_code[0], kbd_code[1], kbd_code[2], kbd_code[3], kbd_code[4], kbd_code[5]);
    tud_hid_n_keyboard_report(ITF_NUM_HID, REPORT_ID_KEYBOARD, kbd_mod, kbd_code);
    // clear changed flag.
    kbd_changed = false;
}

// Invoked when received GET_REPORT control request
//
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request.
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    printf("unhandle HID get report: instance=%d id=%d type=%d buf[0]=%02x len=%d\n", instance, report_id, report_type, buffer[0], reqlen);
    return reqlen;
}

__attribute__((weak)) void kbd_indicator_changed(kbd_indicator_t v) {
    printf("kbd_indicator_changed: %d %d %d %d %d\n", v.num, v.caps, v.scroll, v.compose, v.kana);
}

__attribute__((weak)) bool kbd_handle_via_command(uint8_t *cmd, uint8_t *data, uint16_t len) {
    printf("kbd_handle_via_command: cmd=%02x size=%d\n", cmd, len);
    return false;
}

// Invoked when received SET_REPORT control request or received data on OUT
// endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
    // handle keyboard's LED status report.
    if (instance == ITF_NUM_HID && report_id == REPORT_ID_KEYBOARD && report_type == HID_REPORT_TYPE_OUTPUT) {
        uint8_t status = 0;
        if (bufsize == 2) {
            status = buffer[1];
        }
        kbd_indicator_t next = {};
        if (status & KEYBOARD_LED_NUMLOCK) {
            next.num = true;
        }
        if (status & KEYBOARD_LED_CAPSLOCK) {
            next.caps = true;
        }
        if (status & KEYBOARD_LED_SCROLLLOCK) {
            next.scroll = true;
        }
        if (status & KEYBOARD_LED_COMPOSE) {
            next.compose = true;
        }
        if (status & KEYBOARD_LED_KANA) {
            next.kana = true;
        }
        kbd_indicator_changed(next);
        return;
    }

    // handle VIA status report.
    if (instance == ITF_NUM_VIA && report_id == 0 && report_type == HID_REPORT_TYPE_OUTPUT) {
        uint8_t response[CFG_TUD_HID_EP_BUFSIZE];
        memcpy(response, buffer, bufsize);
        if (kbd_handle_via_command(&response[0], &response[1], bufsize - 1)) {
            tud_hid_n_report(ITF_NUM_VIA, report_id, buffer, bufsize);
        }
        return;
    }

    // dump unknown reports.
    printf("unknown HID set report: instance=%d id=%d type=%d size=%d\n", instance, report_id, report_type, bufsize);
    for (uint16_t i = 0; i < bufsize; i++) {
        printf(" %02x", buffer[i]);
        if ((i+1) % 16 == 0 || i == bufsize-1) {
            printf("\n");
        }
    }
}
