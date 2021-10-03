#include "pico/stdlib.h"

#include "config.h"
#include "ledarray.h"

static void update_rainbow(uint t) {
    uint level = 3;
    //uint level = (t / LEDARRAY_NUM) % 7;
    for (int i = 0; i < LEDARRAY_NUM; i++) {
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
    for (int i = 0; i < LEDARRAY_NUM; i++) {
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

typedef void (*pattern)(uint t);

static pattern patterns[] = {
    update_snake,
    update_rainbow,
};

const uint pattern_choice = 1;

void backlight_task(uint64_t now) {
    static uint64_t last = 0;
    static uint32_t state = 0;
    if (now - last < 33 * 1000) {
        return;
    }
    last = now;
    patterns[pattern_choice % count_of(patterns)](state);
    if (ledarray_task()) {
        state++;
    }
}

void backlight_init() {
    ledarray_init();
}
