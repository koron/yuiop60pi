#pragma once

//////////////////////////////////////////////////////////////////////////////
// Functions

typedef struct {
    uint col;
    uint row;
    bool on;
    keycode_t kc;
} action_event_t;

#ifdef __cplusplus
extern "C" {
#endif

void action_perform(uint64_t when, action_event_t *ev);
bool action_layer(uint64_t when, action_event_t *ev);

//----------------------------------------------------------------------------
// Hooks

void action_report_code(uint8_t code, bool on);

#ifdef __cplusplus
}
#endif
