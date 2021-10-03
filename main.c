#include <stdio.h>

#include "pico/stdlib.h"

#include "config.h"
#include "onboard_led.h"
#include "matrix.h"
#include "backlight.h"

int main() {
    setup_default_uart();
    printf("YUIOP60Pi: start\n");

    onboard_led_init();
    matrix_init();
    backlight_init();

    while(true) {
        uint64_t now = time_us_64();
        backlight_task(now);
        matrix_task(now);
        onboard_led_task(now);
        //sleep_ms(5);
    }

    printf("YUIOP60Pi: end\n");
    return 0;
}
