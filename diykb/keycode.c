#include "pico/stdlib.h"

#include "keycode.h"

bool kc_is_hid(keycode_t kc) {
    return kc >= KC_A && kc <= 0xff;
}

bool kc_is_ext(keycode_t kc, keycode_t constcode, keycode_t mask, uint16_t *value) {
    if ((kc & (~mask)) != constcode) {
        return false;
    }
    if (value != NULL) {
        *value = kc & mask;
    }
    return true;
}
