#include <stdio.h>

#include "pico/stdlib.h"

#include "keycode.h"
#include "layer.h"
#include "action.h"

__attribute__((weak)) void action_report_code(uint8_t code, bool on) {
    printf("action_report_code: code=%02x on=%d\n", code, on);
}

__attribute__((weak)) void action_report_oneshot(uint8_t code) {
    printf("action_report_oneshot: code=%02x\n", code);
}

bool action_do_no(uint64_t when, action_event_t *ev) {
    return ev->kc == KC_NO;
}

bool action_do_hid(uint64_t when, action_event_t *ev) {
    if (!kc_is_hid(ev->kc)) {
        return false;
    }
    action_report_code(ev->kc, ev->on);
    return true;
}

bool action_do_layer(uint64_t when, action_event_t *ev) {
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

static action_handler_t *first_handler = NULL;
static action_handler_t **last_handler = NULL;

void action_add_handler(action_handler_t *h) {
    if (h == NULL) {
        return;
    }
    h->next = NULL;
    if (first_handler == NULL) {
        first_handler = h;
        last_handler = &first_handler->next;
    } else {
        *last_handler = h;
        last_handler = &(*last_handler)->next;
    }
}

//////////////////////////////////////////////////////////////////////////////

void action_perform(uint64_t when, action_event_t *ev) {
    //printf("action_perform: when=%llu knum=%d %s kc=%04x\n", when, ev->knum, ev->on ? "ON" : "OFF", ev->kc);
    int n = 0;
    for (action_handler_t *h = first_handler; h != NULL; h = h->next, n++) {
        if (h->fn(when, ev)) {
            //printf("action_perform: did action#%d\n", n);
            return;
        }
    }
}
