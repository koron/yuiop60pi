#pragma once

typedef struct {
    bool num:1;
    bool caps:1;
    bool scroll:1;
    bool compose:1;
    bool kana:1;
} kbd_indicator_t;

#ifdef __cplusplus
extern "C" {
#endif

void kbd_task(uint64_t now);
void kbd_report_code(uint8_t code, bool on);

__attribute__((weak)) void kbd_indicator_changed(kbd_indicator_t v);

__attribute__((weak)) bool kbd_handle_via_command(uint8_t *cmd, uint8_t *data, uint16_t size);

#ifdef __cplusplus
}
#endif
