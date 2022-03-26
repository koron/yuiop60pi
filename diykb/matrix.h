#pragma once

//////////////////////////////////////////////////////////////////////////////
// Options (configure in your config.h)

// MATRIX_SCAN_PERFORMANCE_COUNT enables scan performance counter. It shows
// count of matrix scan to console (UART0) each a second.
//#define MATRIX_SCAN_PERFORMANCE_COUNT

// MATRIX_SCAN_INTERVAL specify interval of matrix scan in micro-seconds.
// When undefined, scan interval is disabled (maximum scan).
//#define MATRIX_SCAN_INTERVAL 499

// MATRIX_ROW_SELECT_DELAY is interval after a row selected on scan, in
// micro-seconds. Default is 1.
//#define MATRIX_ROW_SELECT_DELAY 1

// MATRIX_DEBOUNCE_USEC is inhibition interval (micro-seconds) for changing
// status of each keys. Default is 10000 (10 millseconds)
//#define MATRIX_DEBOUNCE_USEC      (10*1000)

//////////////////////////////////////////////////////////////////////////////
// Functions

#ifdef __cplusplus
extern "C" {
#endif

void matrix_init();

void matrix_task(uint64_t now);

//----------------------------------------------------------------------------
// Hooks

// matrix_changed is callbacked when matrix scanner detects a change
// of key state.
void matrix_changed(uint64_t when, uint knum, bool on);

// matrix_suppressed is callbacked when matrix scanner detects
// chattering and suppresses the change of a key state.
void matrix_suppressed(uint64_t when, uint knum, bool on, uint64_t last, uint64_t elapsed);

#ifdef __cplusplus
}
#endif
