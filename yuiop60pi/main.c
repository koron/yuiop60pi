#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "bsp/board.h"
#include "tusb.h"

#include "config.h"
#include "diykb/backlight.h"
#include "diykb/matrix.h"
#include "diykb/kbd.h"
#include "diykb/ledarray.h"

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

static void core1_main(void) {
    printf("YUIOP60Pi: core1 launched\n");
    while(true) {
        uint64_t now = time_us_64();
        backlight_task(now);
        ledarray_task(now);
    }
}

int main() {
    setup_default_uart();
    printf("\nYUIOP60Pi: start\n");

    ledarray_init();
    backlight_init();
    tusb_init();
    kbd_init();
    matrix_init();

    multicore_launch_core1(core1_main);

    while(true) {
        uint64_t now = time_us_64();
        matrix_task(now);
        kbd_task(now);
        tud_task();
    }
}
