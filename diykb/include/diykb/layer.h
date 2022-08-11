#pragma once

extern uint32_t layer_state;

#ifdef __cplusplus
extern "C" {
#endif

bool layer_is_enabled(int layer);
void layer_set_enable(int layer);
void layer_set_disable(int layer);
void layer_set(int layer, bool enable);
void layer_toggle(int layer);

keycode_t layer_get_keycode(uint knum);

#ifdef __cplusplus
}
#endif
