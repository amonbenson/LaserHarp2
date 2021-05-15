#pragma once


#include "config.h"


#define CENTER_NOTE         64


#define LAYOUT_XY           0
#define LAYOUT_TRI          1

#define SCALE_CHROMATIC     0


int lookup_layout_tri[8][8] = {
    { 63, 62, 60, 57, 53, 48, 42, 35 },
    { 61, 59, 56, 52, 47, 41, 34, 27 },
    { 58, 55, 51, 46, 40, 33, 26, 20 },
    { 54, 50, 45, 39, 32, 25, 19, 14 },
    { 49, 44, 38, 31, 24, 18, 13, 9  },
    { 43, 37, 30, 23, 17, 12, 8,  5  },
    { 36, 29, 22, 16, 11, 7,  4,  2  },
    { 28, 21, 15, 10, 6,  3,  1,  0  },
};

int apply_layout(int layout, int x, int y) {
    // XY-Layout
    if (layout == LAYOUT_XY) {
        return x + y * HARP_SIZE_X;
    }

    // Triangular Layout
    if (layout == LAYOUT_TRI) {
        return lookup_layout_tri[7 - y][x];
    }

    return 0;
}

int apply_scale(int scale, int i) {
    // Chromatic Scale
    if (scale == SCALE_CHROMATIC) {
        return i - HARP_SIZE_X * HARP_SIZE_Y / 2 + CENTER_NOTE;
    }

    return 0;
}
