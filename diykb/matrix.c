#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "config.h"

#ifndef COL_PINS
# pragma GCC error "COL_PINS should be defined in config.h"
# define COL_PINS   {}
#endif
#ifndef COL_NUM
# pragma GCC error "COL_NUM should be defined in config.h"
# define COL_NUM    0
#elif COL_NUM > 32
# pragma GCC error "COL_NUM should be <= 32"
#endif
#ifndef ROW_PINS
# pragma GCC error "ROW_PINS should be defined in config.h"
# define ROW_PINS   {}
#endif
#ifndef ROW_NUM
# pragma GCC error "ROW_NUM should be defined in config.h"
# define ROW_NUM    0
#endif

#ifndef MATRIX_ROW_SELECT_DELAY
# define MATRIX_ROW_SELECT_DELAY 1
#endif

// MATRIX_DEBOUNCE_USEC is inhibition interval (us) for changing status of each
// keys.
#ifndef MATRIX_DEBOUNCE_USEC
# define MATRIX_DEBOUNCE_USEC      (10*1000)
#endif

static const uint col_pins[] = COL_PINS;
static const uint row_pins[] = ROW_PINS;

typedef struct {
    bool     on;
    uint64_t last;
} scan_state;

static scan_state matrix_states[COL_NUM * ROW_NUM];

// matrix_get_state fetches whole matrix status (on/off) as bits form, into
// limited buffer in "size" bytes.
void matrix_get_state(uint8_t *data, uint16_t size) {
    const int ROW_BYTES = (COL_NUM + 7) / 8; // 1-4
    uint x = 0;
    uint16_t w = 0;
    for (uint nrow = 0; nrow < ROW_NUM; nrow++) {
        uint32_t curr = 0;
        for (uint ncol = 0; ncol < COL_NUM; ncol++) {
            if (matrix_states[x].on) {
                curr |= 1 << ncol;
            }
            x++;
        }
        curr <<= 8 * (4 - ROW_BYTES);
        for (uint8_t i = 0; i < ROW_BYTES && w < size; i++) {
            data[w++] = (curr >> 24) & 0xff;
            curr <<= 8;
        }
    }
}

__attribute__((weak)) void matrix_changed(uint ncol, uint nrow, bool on, uint64_t when) {
    printf("matrix_changed: col=%d row=%d %s when=%llu\n", ncol, nrow, on ? "ON" : "OFF", when);
}

__attribute__((weak)) void matrix_suppressed(uint ncol, uint nrow, bool on, uint64_t when, uint64_t last, uint64_t elapsed) {
    printf("matrix_suppressed: col=%d row=%d %s when=%llu last=%llu elapsed=%llu\n", ncol, nrow, on ? "ON" : "OFF", when, last, elapsed);
}

static void performance_count(uint64_t now) {
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
    uint x = 0;
    for (uint nrow = 0; nrow < ROW_NUM; nrow++) {
        // select a row, wait a bit, fetch columns status, unselect a row.
        uint pin = row_pins[nrow];
        gpio_set_dir(pin, GPIO_OUT);
        sleep_us(MATRIX_ROW_SELECT_DELAY);
        uint32_t bits = gpio_get_all();
        gpio_set_dir(pin, GPIO_IN);
        // parse columns status as matrix states.
        for (uint ncol = 0; ncol < COL_NUM; ncol++) {
            bool on = (bits & (1ul << col_pins[ncol])) == 0;
            if (on != matrix_states[x].on) {
                uint64_t elapsed = now - matrix_states[x].last;
                if (elapsed >= MATRIX_DEBOUNCE_USEC) {
                    matrix_states[x].on = on;
                    matrix_states[x].last = now;
                    matrix_changed(ncol, nrow, on, now);
                } else {
                    matrix_suppressed(ncol, nrow, on, now, matrix_states[x].last, elapsed);
                }
            }
            x++;
        }
    }
    performance_count(now);
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
    //printf("matrix_init: sizeof(matrix_states)=%d\n", sizeof(matrix_states));
    // setup pins of columns.
    for (int i = 0; i < COL_NUM; i++) {
        matrix_gpio_init(col_pins[i]);
    }
    // setup pins of rows.
    for (int i = 0; i < ROW_NUM; i++) {
        matrix_gpio_init(row_pins[i]);
    }
    memset(matrix_states, 0, sizeof(matrix_states));
}
