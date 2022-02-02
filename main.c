#include <stdio.h>

#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"

#include "config.h"
#include "backlight.h"
#include "matrix.h"
#include "layer.h"
#include "kbd.h"
#include "ledarray.h"

void matrix_changed(uint ncol, uint nrow, bool on, uint64_t when) {
    uint8_t code = layer_get_code(ncol, nrow, on);
    if (code != 0) {
        //printf("report code (%-3s): %02x row=%d col=%-2d when=%llu\n", on ? "ON" : "OFF", code, ncol, nrow, when);
        kbd_report_code(code, on);
    }
}

void kbd_indicator_changed(kbd_indicator_t v) {
    // apply lock indicators status to 1st LED.
    uint8_t r = 0, g = 0, b = 0;
    if (v.num) {
        b = 0xff;
    }
    if (v.caps) {
        r = 0xff;
    }
    if (v.scroll) {
        g = 0xaa;
    }
    ledarray_set_rgb(0, r, g, b);
}

int main() {
    setup_default_uart();
    printf("\nYUIOP60Pi: start\n");
    ledarray_init();
    backlight_init();
    tusb_init();
    kbd_init();
    matrix_init();

    while(true) {
        uint64_t now = time_us_64();
        matrix_task(now);
        kbd_task(now);
        tud_task();
        backlight_task(now);
        ledarray_task(now);
    }
}
