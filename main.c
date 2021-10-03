#include <stdio.h>

#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"

#include "config.h"
#include "onboard_led.h"
#include "matrix.h"
#include "backlight.h"

// Invoked when received GET_REPORT control request
//
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request.
__attribute__((weak)) uint16_t hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    printf("hid_get_report_cb(weak stub): instance=%d id=%d type=%d buf=%p len=%d\n", instance, report_id, report_type, buffer, reqlen);
    return 0;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    return hid_get_report_cb(instance, report_id, report_type, buffer, reqlen);
}

// Invoked when received SET_REPORT control request or received data on OUT
// endpoint ( Report ID = 0, Type = 0 )
__attribute__((weak)) void hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
    printf("hid_set_report_cb(weak stub): instance=%d id=%d type=%d buf=%p size=%d\n", instance, report_id, report_type, buffer, bufsize);
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
    hid_set_report_cb(instance, report_id, report_type, buffer, bufsize);
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
        backlight_task(now);
        matrix_task(now);
        onboard_led_task(now);
        // TODO: work as keyboard
        tud_task();
    }

    printf("YUIOP60Pi: end\n");
    return 0;
}
