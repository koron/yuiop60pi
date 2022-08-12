#pragma once

#include "diykb/action.h"

#ifdef __cplusplus
extern "C" {
#endif

void light_init();
void light_task(uint64_t now);
bool light_do_action(uint64_t when, action_event_t *ev);

void light_set_clipping(int start, int num);

void light_disable(void);
void light_enable(void);
bool light_is_enable(void);

int light_get_pattern_max(void);
int light_get_pattern(void);
void light_set_pattern(int pattern);

#ifdef __cplusplus
}
#endif
