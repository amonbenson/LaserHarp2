#pragma once

#include <Bounce2.h>
#include <MIDI.h>
#include "config.h"


struct harp_state {
    int sx;
    int sy;
    bool active;
    uint8_t note;
    unsigned long start_time;
};


void harp_begin();

void harp_update();

void harp_next_scale();
