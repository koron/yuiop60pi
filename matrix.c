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
#endif
#ifndef ROW_PINS
# pragma GCC error "ROW_PINS should be defined in config.h"
# define ROW_PINS   {}
#endif
#ifndef ROW_NUM
# pragma GCC error "ROW_NUM should be defined in config.h"
# define ROW_NUM    0
#endif

// SCANSHIFT_INTERVAL is interval for each row scanning of matrix (us).
#ifndef SCANSHIFT_INTERVAL
# define SCANSHIFT_INTERVAL 10
#endif

// DEBOUNCE_USEC is inhibition interval (us) for changing status of each
// keys.
#ifndef DEBOUNCE_USEC
# define DEBOUNCE_USEC      (10*1000)
#endif

static const uint col_pins[] = COL_PINS;
static const uint row_pins[] = ROW_PINS;

static uint32_t row_scanmask;
static uint32_t row_scanval[ROW_NUM];
static uint32_t col_masks[COL_NUM];

typedef struct {
    bool     on;
    uint64_t last;
} scan_state;

static scan_state matrix_states[COL_NUM * ROW_NUM];

__attribute__((weak)) void matrix_changed(uint ncol, uint nrow, bool on, uint64_t when) {
    printf("matrix_changed: col=%d row=%d %s when=%llu\n", ncol, nrow, on ? "ON" : "OFF", when);
}

__attribute__((weak)) void matrix_suppressed(uint ncol, uint nrow, bool on, uint64_t when, uint64_t last, uint64_t elapsed) {
    printf("matrix_suppressed: col=%d row=%d %s when=%llu last=%llu elapsed=%llu\n", ncol, nrow, on ? "ON" : "OFF", when, last, elapsed);
}

// matrix_task scan whole switch matrix.
void matrix_task(uint64_t now) {
    uint x = 0;
    for (uint nrow = 0; nrow < ROW_NUM; nrow++) {
        gpio_put_masked(row_scanmask, row_scanval[nrow]);
        sleep_us(SCANSHIFT_INTERVAL);
        uint32_t bits = gpio_get_all();
        for (uint ncol = 0; ncol < COL_NUM; ncol++) {
            bool on = (bits & col_masks[ncol]) == 0;
            if (on != matrix_states[x].on) {
                uint64_t elapsed = now - matrix_states[x].last;
                if (elapsed >= DEBOUNCE_USEC) {
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
}

void matrix_init() {
    //printf("matrix_init: sizeof(matrix_states)=%d\n", sizeof(matrix_states));
    // setup pins of columns.
    for (int i = 0; i < COL_NUM; i++) {
        uint io = col_pins[i];
        gpio_init(io);
        gpio_set_dir(io, GPIO_IN);
        gpio_pull_up(io);
        col_masks[i] = 1ul << io;
    }
    // setup pins of rows.
    uint32_t mask = 0;
    for (int i = 0; i < ROW_NUM; i++) {
        uint io = row_pins[i];
        gpio_init(io);
        gpio_set_dir(io, GPIO_OUT);
        row_scanval[i] = ~(1ul << io);
        mask |= 1ul << io;
    }
    // compose scan mask.
    row_scanmask = mask;
    for (int i = 0; i < ROW_NUM; i++) {
        row_scanval[i] &= mask;
    }
    memset(matrix_states, 0, sizeof(matrix_states));
}
