#include "pico/stdlib.h"

#include "config.h"
#include "nvm.h"

void rgb_act_tog(void) {
    // FIXME: better looks
    nvm.lighting.rgblight.enable = !nvm.lighting.rgblight.enable;
    nvm_set_modified();
}

void rgb_act_mode_forward(void) {
    // TODO: implement me
}

void rgb_act_mode_reverse(void) {
    // TODO: implement me
}

void rgb_act_hui(void) {
    // TODO: implement me
}

void rgb_act_hud(void) {
    // TODO: implement me
}

void rgb_act_sai(void) {
    // TODO: implement me
}

void rgb_act_sad(void) {
    // TODO: implement me
}

void rgb_act_vai(void) {
    // TODO: implement me
}

void rgb_act_vad(void) {
    // TODO: implement me
}

void rgb_act_spi(void) {
    // TODO: implement me
}

void rgb_act_spd(void) {
    // TODO: implement me
}

void rgb_act_mode_plain(void) {
    // TODO: implement me
}

void rgb_act_mode_breathe(void) {
    // TODO: implement me
}

void rgb_act_mode_rainbow(void) {
    // TODO: implement me
}

void rgb_act_mode_swirl(void) {
    // TODO: implement me
}

void rgb_act_mode_snake(void) {
    // TODO: implement me
}

void rgb_act_mode_knight(void) {
    // TODO: implement me
}

void rgb_act_mode_xmas(void) {
    // TODO: implement me
}

void rgb_act_mode_gradient(void) {
    // TODO: implement me
}

void rgb_act_mode_rgbtest(void) {
    // TODO: implement me
}
