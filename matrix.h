#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void matrix_init();
void matrix_task(uint64_t now);

// matrix_changed is callbacked when matrix scanner detects a change.
__attribute__((weak)) void matrix_changed(uint ncol, uint nrow, bool on, uint64_t when);

// matrix_suppressed is callbacked when matrix scanner detects chattering and
// suppresses the change.
__attribute__((weak)) void matrix_suppressed(uint ncol, uint nrow, bool on, uint64_t when, uint64_t last, uint64_t elapsed);

#ifdef __cplusplus
}
#endif
