#pragma once

//////////////////////////////////////////////////////////////////////////////
// for usb_descriptors.c

#define VENDOR_ID       0x7890
#define PRODUCT_ID      0x8001
#define DEVICE_VER      0x0001
#define MANUFACTURER    koron
#define PRODUCT         YUIOP60Pi

//////////////////////////////////////////////////////////////////////////////
// Global configurations

#define KEY_NUM     60

//////////////////////////////////////////////////////////////////////////////
// for matrix.c

//#define MATRIX_SCAN_PERFORMANCE_COUNT

#define MATRIX_SCAN_INTERVAL 499

//#define MATRIX_ROW_SELECT_DELAY 0

//#define MATRIX_DEBOUNCE_USEC      (10*1000)

//////////////////////////////////////////////////////////////////////////////
// for ledarray.c

#define LEDARRAY_NUM    56
#define LEDARRAY_PIN    9
#define LEDARRAY_PIO    pio0

//////////////////////////////////////////////////////////////////////////////
// for light.c

// TODO:

//////////////////////////////////////////////////////////////////////////////
// development flags
