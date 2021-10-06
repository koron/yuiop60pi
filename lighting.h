#pragma once

typedef struct {
    uint8_t mode;
    uint8_t speed;
    uint8_t hue;
    uint8_t sat;
    uint8_t val;
} rgblight_state_t;

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////
// backlight

void backlight_save(void);

uint8_t backlight_get_brightness(void);

void backlight_set_brightness(uint8_t v);

uint8_t backlight_get_effect(void);

void backlight_set_effect(uint8_t v);

//////////////////////////////////////////////////////////////////////////////
// rgblight

void rgblight_save(void);

void rgblight_get_state(rgblight_state_t *s);

void rgblight_set_state(const rgblight_state_t *s);

#ifdef __cplusplus
}
#endif
