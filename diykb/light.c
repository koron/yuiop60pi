#include "pico/stdlib.h"

#include "config.h"
#include "diykb/ledarray.h"
#include "diykb/light.h"

static bool enable = true;
static int clip_start = 0;
static int clip_end = LEDARRAY_NUM;
static int pattern_choice = 0;

//////////////////////////////////////////////////////////////////////////////
// Lighting patter definitions.

static void update_rainbow(uint t) {
    uint level = 0;
    //uint level = (t / LEDARRAY_NUM) % 7;
    for (int i = clip_start; i < clip_end; i++) {
        uint8_t r = 0, g = 0, b = 0;
        float h = (float)((i + t) % LEDARRAY_NUM)/ (float)LEDARRAY_NUM * 6;
        int phase = (int)h;
        uint8_t f = (uint8_t)(255 * (h - (float)phase));
        switch (phase) {
            default:
            case 0:
                r = 255;
                g = f;
                b = 0;
                break;
            case 1:
                r = 255 - f;
                g = 255;
                b = 0;
                break;
            case 2:
                r = 0;
                g = 255;
                b = f;
                break;
            case 3:
                r = 0;
                g = 255 - f;
                b = 255;
                break;
            case 4:
                r = f;
                g = 0;
                b = 255;
                break;
            case 5:
                r = 255;
                g = 0;
                b = 255 - f;
                break;
        }
        ledarray_set_rgb(i, r >> level, g >> level, b >> level);
    }
}

static void update_snake(uint t) {
    for (int i = clip_start; i < clip_end; i++) {
        uint32_t c = 0;
        uint x = (i + t) % LEDARRAY_NUM;
        if (x < 5) {
            ledarray_set_rgb(i, 255, 0, 0);
        } else if (x < 10) {
            ledarray_set_rgb(i, 0, 255, 0);
        } else if (x < 15) {
            ledarray_set_rgb(i, 0, 0, 255);
        } else {
            ledarray_set_rgb(i, 0, 0, 0);
        }
    }
}

static void update_rgb_test(uint t) {
    uint8_t r = 0, g = 0, b = 0;
    uint8_t v = 0xff;
    switch ((t / 45) % 3) {
        case 0:
            r = v;
            break;
        case 1:
            g = v;
            break;
        case 2:
            b = v;
            break;
    }
    for (int i = clip_start; i < clip_end; i++) {
        ledarray_set_rgb(i, r, g, b);
    }
}

static void update_rgb_breath(uint t) {
    uint8_t r = 0, g = 0, b = 0;
    t = t * 2 / 10;
    uint8_t v = t % 30;
    if (v > 15) {
        v = 30 - v;
    }
    v *= 17;
    switch ((t / 30) % 3) {
        case 0:
            r = v;
            break;
        case 1:
            g = v;
            break;
        case 2:
            b = v;
            break;
    }
    for (int i = clip_start; i < clip_end; i++) {
        ledarray_set_rgb(i, r, g, b);
    }
}

typedef void (*pattern_func)(uint t);

static pattern_func patterns[] = {
    update_rainbow,
    update_snake,
    update_rgb_breath,
    update_rgb_test,
};

//////////////////////////////////////////////////////////////////////////////
// Lighting management API.

void light_init() {
    // FIXME: implement me in future.
}

void light_task(uint64_t now) {
    if (!enable) {
        return;
    }
    static uint64_t last = 0;
    static uint32_t state = 0;
    if (now - last < 33 * 1000) {
        return;
    }
    last = now;
    patterns[pattern_choice](state);
    state++;
}

bool light_do_action(uint64_t when, action_event_t *ev) {
    switch (ev->kc) {
        case LGT_TOG:
            if (ev->on) {
                if (light_is_enable()) {
                    light_disable();
                } else {
                    light_enable();
                }
                return true;
            }
            break;
        case LGT_PAI:
            if (ev->on && enable) {
                pattern_choice = (pattern_choice + 1) % count_of(patterns);
                return true;
            }
            break;
        case LGT_PAD:
            if (ev->on && enable) {
                pattern_choice = (pattern_choice + count_of(patterns) - 1) % count_of(patterns);
                return true;
            }
            break;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////
// Lighting API

void light_set_clipping(int start, int end) {
    if (start < 0 || start > LEDARRAY_NUM) {
        start = 0;
    }
    if (end > LEDARRAY_NUM || end < start) {
        end = LEDARRAY_NUM;
    }
    if (clip_start != start || clip_end != end) {
        // turn off clipped lights
        for (int i = 0; i < start; i++) {
            ledarray_set_rgb(i, 0, 0, 0);
        }
        for (int i = end; i < LEDARRAY_NUM; i++) {
            ledarray_set_rgb(i, 0, 0, 0);
        }
        // update clipping lights info
        clip_start = start;
        clip_end = end;
    }
}

void light_disable(void) {
    for (int i = 0; i < LEDARRAY_NUM; i++) {
        ledarray_set_rgb(i, 0, 0, 0);
    }
    enable = false;
}

void light_enable(void) {
    enable = true;
}

bool light_is_enable(void) {
    return enable;
}

int light_get_pattern_max(void) {
    return count_of(patterns);
}

int light_get_pattern(void) {
    return pattern_choice;
}

void light_set_pattern(int pattern) {
    pattern_choice = MIN(MAX(pattern, count_of(patterns) - 1), 0);
}
