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

Output: `build\yuiop60pi.uf2`

## Pin assign

* Key matrix
    * Scan direction: column to row
    * COLUMNS: 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10
    * ROWS: 8, 7, 6, 5, 4
* LED array: 9

## How to modify for your own keyborads

Edit these files:

* config.h - hardware information
* keymap.c - keymap definition
* main.c - add awesome extra features
