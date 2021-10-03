#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void kbd_task(uint64_t now);
void kbd_report_code(uint8_t code, bool on);

#ifdef __cplusplus
}
#endif
