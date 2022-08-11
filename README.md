# YUIOP60Pi - DIY keyboard with PGA2040 firmware

## Requirements

* [PICO SDK][picosdk] (v1.2.0)
  * cmake
  * compiler or so

[picosdk]:https://github.com/raspberrypi/pico-sdk

## How to build

```console
$ make
```

Output: `build\yuiop60pi\yuiop60pi.uf2`

## Hardware

* Key matrix
    * Scan direction: column to row
    * Column pin: 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10
    * Row pin: 8, 7, 6, 5, 4
* LED array:
    * Data pin: 9
    * [WS2812C-2020][ws2812c]
    * Number: 1 + 55
* Schematics: <https://github.com/koron/yuiop/tree/master/yuiop60pi>

[ws2812c]:https://akizukidenshi.com/catalog/g/gI-15068/

## How to modify for your own keyborads

Edit these files:

* yuiop60pi/config.h - hardware information
* yuiop60pi/keymap.c - keymap definition
* yuiop60pi/main.c - add awesome extra features
* yuiop60pi/CMakeLists.txt

## Supported layer control special codes

* `TO(x)` - Turn on a layer when pressing
* `MO(x)` - Momentary turn a layer on.
* `TG(x)` - Toggle a layer on/off.

Other special keycodes are not supported yet.
