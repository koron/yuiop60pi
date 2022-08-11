#pragma once

typedef struct {
    bool num:1;
    bool caps:1;
    bool scroll:1;
    bool compose:1;
    bool kana:1;
} kbd_indicator_t;

//////////////////////////////////////////////////////////////////////////////
// Functions

#ifdef __cplusplus
extern "C" {
#endif

void kbd_init();
void kbd_task(uint64_t now);

//----------------------------------------------------------------------------
// Hooks

void kbd_indicator_changed(kbd_indicator_t v);

#ifdef __cplusplus
}
#endif
