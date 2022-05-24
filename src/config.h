#pragma once


//#define DEBUG

// general settings
#define HARP_SIZE 8
#define SETUP_PIN 2

// midi settings
#define MIDI_VELOCITY 127
#define MIDI_CHANNEL 1
#define MIDI_DURATION 50 // minimal duration of a note in ms

#define NUM_MIDI_SCALES 2
#define MIDI_SCALES {\
    { \
        { 56, 52, 48, 44, 40, 36, 32, 28 }, \
        { 57, 53, 49, 45, 41, 37, 33, 29 }, \
        { 58, 54, 50, 46, 42, 38, 34, 30 }, \
        { 59, 55, 51, 47, 43, 39, 35, 31 }, \
        { 88, 84, 80, 76, 72, 68, 64, 60 }, \
        { 89, 85, 81, 77, 73, 69, 65, 61 }, \
        { 90, 86, 82, 78, 74, 70, 66, 62 }, \
        { 91, 87, 83, 79, 75, 71, 67, 63 }, \
    }, \
    { \
        { -1, 60, -1, 53, -1, 48, -1, -1 }, \
        { 72, 61, 62, 54, 55, 49, 50, -1 }, \
        { 73, 74, 63, 64, 56, 57, 51, 52 }, \
        { 84, 75, 76, -1, 65, 58, 59, -1 }, \
        { 85, 86, -1, 77, 66, 67, -1, 60 }, \
        { 91, 87, 88, 78, 79, 68, 69, 61 }, \
        { 92, 93, -1, 89, 80, 81, 70, 71 }, \
        { 96, 94, 95, 90, 91, 82, 83, -1 }, \
    } \
}

// sensors
#define NUM_SENSORS (HARP_SIZE * 2)
#define SENSOR_PINS { 4, 18, 19, 15, 9, 14, 16, 17, 7, 6, 12, 10, 5, 8, 13, 11 }
