#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void lighting_init();
void lighting_task(uint64_t now);

void lighting_disable(void);
void lighting_enable(void);
bool lighting_is_enable(void);

#ifdef __cplusplus
}
#endif
