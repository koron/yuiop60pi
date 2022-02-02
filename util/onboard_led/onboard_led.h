#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void onboard_led_init();
void onboard_led_task(uint64_t now);

#ifdef __cplusplus
}
#endif
