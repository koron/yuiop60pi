#pragma once

extern uint32_t ledarray_state[LEDARRAY_NUM];
extern bool ledarray_dirty;

//////////////////////////////////////////////////////////////////////////////
// Functions

#ifdef __cplusplus
extern "C" {
#endif

// ledarray_init() initializes LED array.
// it requires a GPIO, a SM of a PIO, and a DMA channel to work.
void ledarray_init();

// ledarray_task transfers bit data to NeoPixel LED.
// it will do nothign when previos transfer doesn't end.
bool ledarray_task(uint64_t now);

// ledarray_num is number of LED in the array.
static inline int ledarray_num() {
    return LEDARRAY_NUM;
}

// ledarray_set_rgb set color of a LED with RGB.
static inline void ledarray_set_rgb(int i, uint8_t r, uint8_t g, uint8_t b) {
    ledarray_state[i] =
        ((uint32_t)(r) << 16) |
        ((uint32_t)(g) << 24) |
        ((uint32_t)(b) << 8);
    ledarray_dirty = true;
}

//----------------------------------------------------------------------------
// Hooks

// ledarray_resetdelay_completed is callbacked when ledarray_task can be
// called.
void ledarray_resetdelay_completed(void);

#ifdef __cplusplus
}
#endif
