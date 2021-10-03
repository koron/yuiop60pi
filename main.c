#include <stdio.h>

#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"

#include "config.h"
#include "onboard_led.h"
#include "matrix.h"
#include "backlight.h"
#include "usb_descriptors.h"
#include "ledarray.h"

// Invoked when received GET_REPORT control request
//
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request.
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    printf("unhandle HID get report: instance=%d id=%d type=%d buf=%p len=%d\n", instance, report_id, report_type, buffer, reqlen);
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
    // handle keyboard's LED status report.
    if (report_id == REPORT_ID_KEYBOARD && report_type == HID_REPORT_TYPE_OUTPUT) {
        uint8_t r = 0, g = 0, b = 0;
        uint8_t status = 0;
        if (bufsize == 2) {
            status = buffer[1];
        }
        if (status & KEYBOARD_LED_NUMLOCK) {
            b = 0x33;
        }
        if (status & KEYBOARD_LED_CAPSLOCK) {
            r = 0x33;
        }
        if (status & KEYBOARD_LED_SCROLLLOCK) {
            g = 0x22;
        }
        ledarray_set_rgb(0, r, g, b);
        return;
    }

    // dump unknown reports.
    printf("unknown HID set report: instance=%d id=%d type=%d size=%d\n", instance, report_id, report_type, bufsize);
    for (uint16_t i = 0; i < bufsize; i++) {
        printf(" %02x", buffer[i]);
        if ((i+1) % 16 == 0 || i == bufsize-1) {
            printf("\n");
        }
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
        backlight_task(now);
        matrix_task(now);
        onboard_led_task(now);
        // TODO: work as keyboard
        tud_task();
    }

    printf("YUIOP60Pi: end\n");
    return 0;
}
