#pragma once

//////////////////////////////////////////////////////////////////////////////
// Options (configure in your config.h)

// MATRIX_SCAN_PERFORMANCE_COUNT enables scan performance counter. It shows
// count of matrix scan to console (UART0) each a second.
//#define MATRIX_SCAN_PERFORMANCE_COUNT

// MATRIX_SCAN_INTERVAL specify interval of matrix scan in micro-seconds.
//#define MATRIX_SCAN_PERFORMANCE_COUNT 500

//////////////////////////////////////////////////////////////////////////////
// Functions

#ifdef __cplusplus
extern "C" {
#endif

void matrix_init();

void matrix_task(uint64_t now);

void matrix_get_state(uint8_t *data, uint16_t size);

// matrix_changed is callbacked when matrix scanner detects a change.
__attribute__((weak)) void matrix_changed(uint ncol, uint nrow, bool on, uint64_t when);

// matrix_suppressed is callbacked when matrix scanner detects chattering and
// suppresses the change.
__attribute__((weak)) void matrix_suppressed(uint ncol, uint nrow, bool on, uint64_t when, uint64_t last, uint64_t elapsed);

#ifdef __cplusplus
}
#endif
