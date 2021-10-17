#include "hardware/gpio.h"

#include "config.h"

#ifndef ONBOARD_LED_PIN
# define ONBOARD_LED_PIN    25
#endif
#ifndef ONBOARD_LED_INTERVAL
# define ONBOARD_LED_INTERVAL   500 * 1000
#endif

void onboard_led_init() {
    gpio_init(ONBOARD_LED_PIN);
    gpio_set_dir(ONBOARD_LED_PIN, GPIO_OUT);
    gpio_put(ONBOARD_LED_PIN, 1);
}

static uint64_t blink_last = 0;
static bool blink_state = false;

void onboard_led_task(uint64_t now) {
    if (now - blink_last >= ONBOARD_LED_INTERVAL) {
        gpio_put(ONBOARD_LED_PIN, blink_state ? 1 : 0);
        blink_state = !blink_state;
        blink_last = now;
    }
}
