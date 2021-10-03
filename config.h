#pragma once

//////////////////////////////////////////////////////////////////////////////
// for usb_descriptors.c

#define VENDOR_ID       0x7890
#define PRODUCT_ID      0x000a
#define DEVICE_VER      0x0001
#define MANUFACTURER    koron
#define PRODUCT         YUIOP60Pi

//////////////////////////////////////////////////////////////////////////////
// for matrix.c

#define COL_PINS    { 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10 }
#define COL_NUM     15
#define ROW_PINS    { 8, 7, 6, 5, 4 }
#define ROW_NUM     5

//define SCANSHIFT_INTERVAL     10
//define DEBOUNCE_USEC          (10 * 1000)

//////////////////////////////////////////////////////////////////////////////
// for ledarray.c

#define LEDARRAY_NUM    56
#define LEDARRAY_PIN    9
#define LEDARRAY_PIO	pio0

//////////////////////////////////////////////////////////////////////////////
// for keymap.c

#define LAYOUT( \
    K01, K02, K03, K04, K05, K06, K07, K08, K09, K10, K11, K12, K13, K14, K15, \
    K16,   K17, K18, K19, K20, K21, K22, K23, K24, K25, K26, K27, K28,    K29, \
    K30,    K31, K32, K33, K34, K35, K36, K37, K38, K39, K40,  K41,       K42, \
    K43,      K44, K45, K46, K47, K48, K49, K50, K51, K52, K53,      K54, K55, \
           K56,   K57,             K58,                 K59, K60 \
) \
{ \
    {   K01,   K02,   K03,   K04,   K05,   K06,   K07,   K08,   K09,   K10,   K11,   K12,   K13,   K14,   K15 }, \
    {   K16,   K17,   K18,   K19,   K20,   K21,   K22,   K23,   K24,   K25,   K26,   K27,   K28, KC_NO,   K29 }, \
    {   K30,   K31,   K32,   K33,   K34,   K35,   K36,   K37,   K38,   K39,   K40,   K41, KC_NO, KC_NO,   K42 }, \
    {   K43, KC_NO,   K44,   K45,   K46,   K47,   K48,   K49,   K50,   K51,   K52,   K53, KC_NO,   K54,   K55 }, \
    { KC_NO,   K56,   K57, KC_NO, KC_NO, KC_NO,   K58, KC_NO, KC_NO, KC_NO,   K59,   K60, KC_NO, KC_NO, KC_NO }  \
}
