#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "config.h"
#include "layout.h"

#ifndef MATRIX_PIN_SELECT_DELAY
# define MATRIX_PIN_SELECT_DELAY 1
#endif

#ifndef MATRIX_PIN_UNSELECT_DELAY
# define MATRIX_PIN_UNSELECT_DELAY 1
#endif

// MATRIX_DEBOUNCE_USEC is inhibition interval (us) for changing status of each
// keys.
#ifndef MATRIX_DEBOUNCE_USEC
# define MATRIX_DEBOUNCE_USEC      (10*1000)
#endif

typedef struct {
    bool     on:1;
    uint64_t last:63;
} matrix_keystate_t;

matrix_keystate_t matrix_keystates[KEY_NUM] = {0};

__attribute__((weak)) void matrix_changed(uint64_t when, uint knum, bool on) {
    printf("matrix_changed: knum=%d %s when=%llu\n", knum, on ? "ON" : "OFF", when);
}

__attribute__((weak)) void matrix_suppressed(uint64_t when, uint knum, bool on, uint64_t last, uint64_t elapsed) {
    printf("matrix_suppressed: knum=%d %s when=%llu last=%llu elapsed=%llu\n", knum, on ? "ON" : "OFF", when, last, elapsed);
}

static void matrix_set_keystate(uint64_t now, uint knum, bool on) {
    if (on != matrix_keystates[knum].on) {
        uint64_t last = matrix_keystates[knum].last << 1;
        uint64_t elapsed = now - last;
        if (elapsed >= MATRIX_DEBOUNCE_USEC) {
            matrix_keystates[knum].on = on;
            matrix_keystates[knum].last = now >> 1;
            matrix_changed(now, knum, on);
        } else {
            matrix_suppressed(now, knum, on, last, elapsed);
        }
    }
}

static void matrix_scan_keys(uint64_t now) {
    uint32_t scanned = 0;
    uint32_t states[32] = {0};
    for (uint i = 0; i < LAYOUT_MATRIX_MAX; i++) {
        uint8_t p0 = layout_matrix[i][0], p1 = layout_matrix[i][1];
        if ((scanned & (1 << p0)) == 0) {
            gpio_set_dir(p0, GPIO_OUT);
            busy_wait_us_32(MATRIX_PIN_SELECT_DELAY);
            states[p0] = gpio_get_all();
            gpio_set_dir(p0, GPIO_IN);
            scanned |= 1 << p0;
            busy_wait_us_32(MATRIX_PIN_UNSELECT_DELAY);
        }
        bool on = (states[p0] & (1 << p1)) == 0;
        matrix_set_keystate(now, i, on);
    }
}

static void matrix_performance_count(uint64_t now) {
#ifdef MATRIX_SCAN_PERFORMANCE_COUNT
    static uint64_t count = 0;
    static uint64_t last = 0;
    ++count;
    if (now - last > 1000000) {
        printf("matrix: scan performance_count: %llu/sec\n", count);
        count = 0;
        last = now;
    }
#endif
}

// matrix_task scan whole switch matrix.
void matrix_task(uint64_t now) {
#if defined(MATRIX_SCAN_INTERVAL) && MATRIX_SCAN_INTERVAL > 0
    static uint64_t last = 0;
    if (now - last < MATRIX_SCAN_INTERVAL) {
        return;
    }
    last = now;
#endif
    matrix_scan_keys(now);
    matrix_performance_count(now);
}

// matrix_gpio_init initialize a GPIO for matrix as input dir, pull up, and
// output LOW.
static void matrix_gpio_init(uint gpio) {
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
    gpio_put(gpio, false);
}

void matrix_init() {
    //printf("matrix_init: KEY_NUM=%d sizeof(matrix_keystates)=%d\n", KEY_NUM, sizeof(matrix_keystates));

    // setup all pins for GPIO.
    uint32_t inited_pins = 0;
    for (uint i = 0; i < LAYOUT_MATRIX_MAX; i++) {
        uint8_t p0 = layout_matrix[i][0], p1 = layout_matrix[i][1];
        if ((inited_pins & (1 << p0)) == 0) {
            matrix_gpio_init(p0);
            inited_pins |= 1 << p0;
        }
        if ((inited_pins & (1 << p1)) == 0) {
            matrix_gpio_init(p1);
            inited_pins |= 1 << p1;
        }
    }
}
