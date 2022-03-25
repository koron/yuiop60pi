#pragma once

#include "pico.h"

#include "config.h"
#include "keycode.h"

extern const keycode_t keymaps[][ROW_NUM][COL_NUM * 2];

extern const size_t KEYMAP_LAYER_MAX;
