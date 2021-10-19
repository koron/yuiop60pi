#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void backlight_init();
void backlight_task(uint64_t now);

void backlight_disable(void);
void backlight_enable(void);

void backlight_act_on(void);
void backlight_act_off(void);
void backlight_act_dec(void);
void backlight_act_inc(void);
void backlight_act_togg(void);
void backlight_act_step(void);
void backlight_act_brtg(void);

#ifdef __cplusplus
}
#endif
