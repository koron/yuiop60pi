#include <stdio.h>

#include "pico/stdlib.h"

#include "config.h"
#include "matrix.h"
#include "onboard_led.h"

int main()
{
    setup_default_uart();
    printf("YUIOP60Pi: start\n");

    matrix_init();
    onboard_led_init();

    while(true) {
        uint64_t now = time_us_64();
        onboard_led_task(now);
        matrix_task(now);
        //sleep_ms(5);
    }

    printf("YUIOP60Pi: end\n");
    return 0;
}
