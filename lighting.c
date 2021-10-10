#include <stdio.h>

#include "pico/stdlib.h"

#include "nvm.h"
#include "lighting.h"

//////////////////////////////////////////////////////////////////////////////
// backlight

static uint8_t backlight_brightness;
static uint8_t backlight_effect;

void backlight_save(void) {
    // FIXME: implement me.
}

uint8_t backlight_get_brightness(void) {
    return backlight_brightness;
}

void backlight_set_brightness(uint8_t v) {
    printf("backlight_set_brightness: %d\n", v);
    backlight_brightness = v;
    // FIXME: apply change.
}

uint8_t backlight_get_effect(void) {
    return backlight_effect;
}

void backlight_set_effect(uint8_t v) {
    printf("backlight_set_effect: %d\n", v);
    backlight_effect = v;
    // FIXME: apply change.
}

//////////////////////////////////////////////////////////////////////////////
// rgblight

void rgblight_save(void) {
    // FIXME: just apply soon?
    nvm_set_modified();
}

void rgblight_get_state(rgblight_state_t *s) {
    if (s == NULL) {
        return;
    }
    *s = nvm.rgblight;
}

void rgblight_set_state(const rgblight_state_t *s, bool persist) {
    if (s == NULL) {
        return;
    }
    printf("rgblight_set_state: mode=%d speed=%d hue=%d sat=%d val=%d\n", s->mode, s->speed, s->hue, s->sat, s->val);
    nvm.rgblight = *s;
    if (persist) {
        nvm_set_modified();
    }
}

// TODO: implement RGB light feature.
