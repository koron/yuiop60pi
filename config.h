#pragma once

//////////////////////////////////////////////////////////////////////////////
// for usb_descriptors.c

#define VENDOR_ID       0xbeef
#define PRODUCT_ID      0x0001
#define DEVICE_VER      0x0001
#define MANUFACTURER    techmech
#define PRODUCT         OLSK60

//////////////////////////////////////////////////////////////////////////////
// for matrix.c

#define COL_PINS    { 29, 28, 27, 26, 18, 20, 19 }
#define COL_NUM     7
#define ROW_PINS    { 2, 3, 4, 5, 6 }
#define ROW_NUM     5

//define SCANSHIFT_INTERVAL     10
//define DEBOUNCE_USEC          (10 * 1000)

//////////////////////////////////////////////////////////////////////////////
// for ledarray.c

#define LEDARRAY_NUM    1
#define LEDARRAY_PIN    0
#define LEDARRAY_PIO	pio0

//////////////////////////////////////////////////////////////////////////////
// for keymap.c

#define LAYOUT( \
    K01, K06, K11, K16, K20, K25, K31, K35, K40, K44, K49, K54,      \
    K02, K07, K12, K17, K21, K26, K32, K36, K41, K45, K50, K55, K59, \
    K03, K08, K13, K18, K22, K27, K33, K37, K42, K46, K51, K56,      \
    K04, K09, K14, K19, K23, K28, K34, K38, K43, K47, K52, K57, K60, \
    K05, K10, K15     , K24, K30     , K39     , K48, K53, K58, K61  \
) \
{ \
    {   K01,   K06,   K11,   K16,   K20,   K25, KC_NO,   K31,   K35,   K40,   K44,   K49,   K54, KC_NO }, \
    {   K02,   K07,   K12,   K17,   K21,   K26, KC_NO,   K32,   K36,   K41,   K45,   K50,   K55,   K59 }, \
    {   K03,   K08,   K13,   K18,   K22,   K27, KC_NO,   K33,   K37,   K42,   K46,   K51,   K56, KC_NO }, \
    {   K04,   K09,   K14,   K19,   K23,   K28, KC_NO,   K34,   K38,   K43,   K47,   K52,   K57,   K60 }, \
    {   K05,   K10,   K15, KC_NO,   K24,   K30, KC_NO, KC_NO,   K39, KC_NO,   K48,   K53,   K58,   K61 }, \
}

//////////////////////////////////////////////////////////////////////////////
// development flags

//#define MATRIX_SCAN_PERFORMANCE_COUNT

#define MATRIX_SCAN_INTERVAL 499

//#define MATRIX_ROW_SELECT_DELAY 0

//#define MATRIX_DEBOUNCE_USEC      (10*1000)
