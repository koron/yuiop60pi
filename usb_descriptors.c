#include <stdio.h>

#include "pico/unique_id.h"
#include "tusb.h"
#include "usb_descriptors.h"

#include "config.h"

//////////////////////////////////////////////////////////////////////////////
// Device Descriptor
tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = VENDOR_ID,
    .idProduct          = PRODUCT_ID,
    .bcdDevice          = DEVICE_VER,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

uint8_t const * tud_descriptor_device_cb(void) {
    return (uint8_t const *)&desc_device;
}

//////////////////////////////////////////////////////////////////////////////
// HID Report Descriptor
uint8_t const desc_hid_report[] =
{
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_ID_KEYBOARD)),
    //TUD_HID_REPORT_DESC_MOUSE   (HID_REPORT_ID(REPORT_ID_MOUSE   )),
};

uint8_t const * tud_hid_descriptor_report_cb(uint8_t itf) {
    return desc_hid_report;
}

//////////////////////////////////////////////////////////////////////////////
// Configuration Descriptor

enum {
    ITF_NUM_HID,
    ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

#define EPNUM_HID   0x81

uint8_t const desc_configuration[] = {
    // Config number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 5)
};

uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
    return desc_configuration;
}

//////////////////////////////////////////////////////////////////////////////
// String Descriptors

#define LANGUAGE_ID_ENG     0x0409

// convert to char string
#define STR(s)      XSTR(s)
#define XSTR(s)     #s
// convert to char16_t (uint16_t) string
#define uSTR(s)     XuSTR(s)
#define XuSTR(s)    u## #s

#define DESC_STRING_LEN(n)  (2 + (n)*2)

static tusb_desc_string_t LanguageString = {
    .bLength            = DESC_STRING_LEN(1),
    .bDescriptorType    = TUSB_DESC_STRING,
    .unicode_string     = { LANGUAGE_ID_ENG },
};

static tusb_desc_string_t ManufacturerString = {
    .bLength            = DESC_STRING_LEN(sizeof(STR(MANUFACTURER)) - 1),
    .bDescriptorType    = TUSB_DESC_STRING,
    .unicode_string     = uSTR(MANUFACTURER),
};

static tusb_desc_string_t ProductString = {
    .bLength            = DESC_STRING_LEN(sizeof(STR(PRODUCT)) - 1),
    .bDescriptorType    = TUSB_DESC_STRING,
    .unicode_string     = uSTR(PRODUCT),
};

static tusb_desc_string_t SerialString = {
    .bLength            = DESC_STRING_LEN(0),
    .bDescriptorType    = TUSB_DESC_STRING,
    .unicode_string     = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

static tusb_desc_string_t* string_descs[] = {
    &LanguageString,
    &ManufacturerString,
    &ProductString,
    &SerialString,
};

static void setupSerialNumber() {
    char uid[PICO_UNIQUE_BOARD_ID_SIZE_BYTES*2 + 1];
    pico_get_unique_board_id_string(uid, sizeof(uid));
    printf("unique board id: %s\n", uid);
    for (int i = 0; i < PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2; i++) {
        SerialString.unicode_string[i] = uid[i];
    }
    SerialString.bLength = DESC_STRING_LEN(PICO_UNIQUE_BOARD_ID_SIZE_BYTES*2);

}

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    if (index == 3 && SerialString.unicode_string[0] == 0) {
        setupSerialNumber();
    }
    if (index < count_of(string_descs)) {
        return (uint16_t const *)string_descs[index];
    }
    return NULL;
}