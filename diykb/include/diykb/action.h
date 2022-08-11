#pragma once

//////////////////////////////////////////////////////////////////////////////
// Functions

typedef struct {
    uint knum;
    bool on;
    keycode_t kc;
} action_event_t;

struct action_handler_s;

typedef bool (*action_handler_fn)(uint64_t when, action_event_t *ev);

typedef struct action_handler_s {
    action_handler_fn fn;
    struct action_handler_s *next;
} action_handler_t;

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// API

void action_add_handler(action_handler_t *h);

void action_perform(uint64_t when, action_event_t *ev);

//----------------------------------------------------------------------------
// Actions

bool action_do_no(uint64_t when, action_event_t *ev);
bool action_do_hid(uint64_t when, action_event_t *ev);
bool action_do_layer(uint64_t when, action_event_t *ev);

//----------------------------------------------------------------------------
// Hooks

void action_report_code(uint8_t code, bool on);

void action_report_oneshot(uint8_t code);

#ifdef __cplusplus
}
#endif
