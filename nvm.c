#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/time.h"

#include "nvm.h"

nvm_block_t nvm;

static uint64_t modified = 0;

void nvm_init(void) {
    printf("sizeof(nvm_block_t)=%d\n", sizeof(nvm_block_t));
    memset(&nvm, 0, sizeof(nvm));
    // TODO: read from XIP_BASE
}

void nvm_task(uint64_t now) {
    // TODO: write to Flash
}

void nvm_set_modified(void) {
    modified = time_us_64();
}
