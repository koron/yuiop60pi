#include <stdio.h>

#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "bsp/board_api.h"
#include "tusb.h"

#include "config.h"
#include "diykb/light.h"
#include "diykb/matrix.h"
#include "diykb/kbd.h"
#include "diykb/ledarray.h"

bi_decl(bi_program_version_string("0.4.0-alpha.3"));

bi_decl(bi_program_feature("USB Keyboard"));
bi_decl(bi_program_feature("NeoPixel LED array"));
bi_decl(bi_pin_mask_with_name(0x000001f0, "Row pins (OUT)"));
bi_decl(bi_pin_mask_with_name(0x01fffc00, "Column pins (IN)"));
bi_decl(bi_1pin_with_name(LEDARRAY_PIN, "WS2812C-2020 x56"));

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
        light_task(now);
        ledarray_task(now);
    }
}

int main() {
    setup_default_uart();
    printf("\nYUIOP60Pi: start\n");

    ledarray_init();
    light_init();
    light_set_clipping(1, LEDARRAY_NUM);
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
