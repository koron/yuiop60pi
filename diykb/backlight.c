#include "pico/stdlib.h"

#include "config.h"
#include "ledarray.h"

static void update_rainbow(uint t) {
    uint level = 1;
    //uint level = (t / LEDARRAY_NUM) % 7;
    for (int i = 1; i < LEDARRAY_NUM; i++) {
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
    for (int i = 1; i < LEDARRAY_NUM; i++) {
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
    uint8_t v = 0x08;
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
    for (int i = 1; i < LEDARRAY_NUM; i++) {
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
    for (int i = 1; i < LEDARRAY_NUM; i++) {
        ledarray_set_rgb(i, r, g, b);
    }
}

typedef void (*pattern)(uint t);

static pattern patterns[] = {
    update_snake,
    update_rainbow,
    update_rgb_test,
    update_rgb_breath
};

static bool enable = true;

const uint pattern_choice = 1;

static void rgb_test(uint64_t now) {
    const uint8_t v = 0x10;
    const uint64_t msec = 2000;

    static uint64_t last = 0;
    static uint8_t state = 0;
    if (now - last < msec * 1000) {
        return;
    }
    last = now;

    for (int i = 1; i < LEDARRAY_NUM; i++) {
        uint8_t r = 0, g = 0, b = 0;
        switch (state) {
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
        ledarray_set_rgb(i, r, g, b);
    }
    state = (state + 1) % 3;
}

void backlight_task(uint64_t now) {
    if (!enable) {
        return;
    }
#if 1
    static uint64_t last = 0;
    static uint32_t state = 0;
    if (now - last < 33 * 1000) {
        return;
    }
    last = now;
    patterns[pattern_choice % count_of(patterns)](state);
    state++;
#else
    rgb_test(now);
#endif
}

void backlight_init() {
    // FIXME: implement me in future.
}

void backlight_disable(void) {
    for (int i = 1; i < LEDARRAY_NUM; i++) {
        ledarray_set_rgb(i, 0, 0, 0);
    }
    enable = false;
}

void backlight_enable(void) {
    enable = true;
}
