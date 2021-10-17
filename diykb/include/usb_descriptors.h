#pragma once

// Interface ID
enum {
    ITF_NUM_HID,
    ITF_NUM_VIA,
    ITF_NUM_TOTAL
};

// HID report ID
enum {
    REPORT_ID_KEYBOARD  = 1,
    REPORT_ID_VIA       = 2,
};

//////////////////////////////////////////////////////////////////////////////
// VIA

#define VIA_EPSIZE      32

#define TUD_HID_REPORT_DESC_VIA() \
    HID_USAGE_PAGE_N ( 0xff60, 2                  ),\
    HID_USAGE        ( 0x61                       ),\
    HID_COLLECTION   ( HID_COLLECTION_APPLICATION ),\
      /* Data to host (input) */\
      HID_USAGE        ( 0x62                                   ),\
      HID_LOGICAL_MIN  ( 0x00                                   ),\
      HID_LOGICAL_MAX  ( 0xff                                   ),\
      HID_REPORT_SIZE  ( 0x08                                   ),\
      HID_REPORT_COUNT ( VIA_EPSIZE                             ),\
      HID_INPUT        ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),\
      /* Data from host (output) */\
      HID_USAGE        ( 0x63                                   ),\
      HID_LOGICAL_MIN  ( 0x00                                   ),\
      HID_LOGICAL_MAX  ( 0xff                                   ),\
      HID_REPORT_SIZE  ( 0x08                                   ),\
      HID_REPORT_COUNT ( VIA_EPSIZE                             ),\
      HID_OUTPUT       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),\
    HID_COLLECTION_END
