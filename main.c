#include <stdio.h>

#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"

#include "config.h"
#include "backlight.h"
#include "matrix.h"
#include "layer.h"
#include "kbd.h"
#include "usb_descriptors.h"
#include "onboard_led.h"

void matrix_changed(uint ncol, uint nrow, bool on, uint64_t when) {
    uint8_t code = layer_get_code(ncol, nrow, on);
    if (code != 0) {
        kbd_report_code(code, on);
    }
}

int main() {
    setup_default_uart();
    printf("YUIOP60Pi: start\n");

    onboard_led_init();
    matrix_init();
    backlight_init();
    tusb_init();

    while(true) {
        uint64_t now = time_us_64();
        onboard_led_task(now);
        backlight_task(now);
        matrix_task(now);
        kbd_task(now);
        tud_task();
    }

    printf("YUIOP60Pi: end\n");
    return 0;
}
