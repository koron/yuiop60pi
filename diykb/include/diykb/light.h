#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void light_init();
void light_task(uint64_t now);

void light_disable(void);
void light_enable(void);
bool light_is_enable(void);

void light_set_clipping(int start, int num);

#ifdef __cplusplus
}
#endif
