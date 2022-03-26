#pragma once

//////////////////////////////////////////////////////////////////////////////
// for usb_descriptors.c

#define VENDOR_ID       0x7890
#define PRODUCT_ID      0x000a
#define DEVICE_VER      0x0001
#define MANUFACTURER    koron
#define PRODUCT         YUIOP60Pi

//////////////////////////////////////////////////////////////////////////////
// Global configurations

#define KEY_NUM     60

//////////////////////////////////////////////////////////////////////////////
// for matrix.c

enum {
    R00 = 8,
    R01 = 7,
    R02 = 6,
    R03 = 5,
    R04 = 4,

    C00 = 24,
    C01 = 23,
    C02 = 22,
    C03 = 21,
    C04 = 20,
    C05 = 19,
    C06 = 18,
    C07 = 17,
    C08 = 16,
    C09 = 15,
    C10 = 14,
    C11 = 13,
    C12 = 12,
    C13 = 11,
    C14 = 10,
};

#define MATRIX_KEYS { \
    {R00,C00},{R00,C01},{R00,C02},{R00,C03},{R00,C04},{R00,C05},{R00,C06},{R00,C07},{R00,C08},{R00,C09},{R00,C10},{R00,C11},{R00,C12},{R00,C13},{R00,C14}, \
    {R01,C00}     ,{R01,C01},{R01,C02},{R01,C03},{R01,C04},{R01,C05},{R01,C06},{R01,C07},{R01,C08},{R01,C09},{R01,C10},{R01,C11},{R01,C12},     {R01,C14}, \
    {R02,C00}        ,{R02,C01},{R02,C02},{R02,C03},{R02,C04},{R02,C05},{R02,C06},{R02,C07},{R02,C08},{R02,C09},{R02,C10},{R02,C11},            {R02,C14}, \
    {R03,C00}             ,{R03,C02},{R03,C03},{R03,C04},{R03,C05},{R03,C06},{R03,C07},{R03,C08},{R03,C09},{R03,C10},{R03,C11},   {R03,C13}    ,{R03,C14}, \
                    {R04,C01},  {R04,C02}  ,                          {R04,C06}                         ,  {R04,C10}  ,{R04,C11}                           \
}

//define SCANSHIFT_INTERVAL     10
//define DEBOUNCE_USEC          (10 * 1000)

//////////////////////////////////////////////////////////////////////////////
// for ledarray.c

#define LEDARRAY_NUM    56
#define LEDARRAY_PIN    9
#define LEDARRAY_PIO    pio0

//////////////////////////////////////////////////////////////////////////////
// development flags

//#define MATRIX_SCAN_PERFORMANCE_COUNT

#define MATRIX_SCAN_INTERVAL 499

//#define MATRIX_ROW_SELECT_DELAY 0

//#define MATRIX_DEBOUNCE_USEC      (10*1000)
