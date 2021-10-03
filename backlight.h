#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void backlight_init();
void backlight_task(uint64_t now);

#ifdef __cplusplus
}
#endif
