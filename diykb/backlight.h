#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void backlight_init();
void backlight_task(uint64_t now);

void backlight_disable(void);
void backlight_enable(void);
bool backlight_is_enable(void);

#ifdef __cplusplus
}
#endif
