#include <stdio.h>

#include "pico/stdlib.h"

#include "keycodes.h"
#include "layer.h"
#include "action.h"

__attribute__((weak)) void action_report_code(uint8_t code, bool on) {
    printf("action_report_code: code=%02x on=%d\n", code, on);
}

bool action_layer(uint64_t when, action_event_t *ev) {
    uint16_t layer = 0;
    // turn on layer when pressed. (TO)
    if (kc_is_ext(ev->kc, KCX_TO, 0x1f, &layer)) {
        if (ev->on) {
            layer_state = 0;
            layer_set_enable((int)layer);
        }
        return true;
    }
    // momentary turn layer on. (MO)
    if (kc_is_ext(ev->kc, KCX_MOMENTARY, 0x1f, &layer)) {
        layer_set((int)layer, ev->on);
        return true;
    }
    // toggle layer on/off. (TG)
    if (kc_is_ext(ev->kc, KCX_TOGGLE_LAYER, 0x1f, &layer)) {
        if (ev->on) {
            layer_toggle((int)layer);
        }
        return true;
    }
    // TODO: implement other layer operations.
    return false;
}

//////////////////////////////////////////////////////////////////////////////

void action_perform(uint64_t when, action_event_t *ev) {
    if (ev->kc == KC_NO) {
        return;
    }
    //printf("action_perform: when=%llu col=%d row=%d %s kc=%04x\n", when, ev->col, ev->row, ev->on ? "ON" : "OFF", ev->kc);
    if (kc_is_hid(ev->kc)) {
        action_report_code(ev->kc, ev->on);
        return;
    }
    if (action_layer(when, ev)) {
        return;
    }
    // no actions mapped.
    // FIXME: add more actions.
}
