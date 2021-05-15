#pragma once


// Serial debugging and midi settings
//#define DEBUG
//#define MIDI_CUSTOM_BAUD_RATE

// Laserharp dimensions. With the 45 deg rotation, x is the left side and y the right side
#define HARP_SIZE_X 8
#define HARP_SIZE_Y 8
#define NUM_SENSORS (HARP_SIZE_X + HARP_SIZE_Y) // = HARP_SIZE_X + HARP_SIZE_Y

// Sensors from left (x diagonal) to right (y diagonal)
#define SENSOR_PINS { 4, 18, 19, 15, 9, 14, 16, 17, 7, 6, 12, 10, 5, 8, 13, 11 }
#define SENSOR_DEBOUNCE 1

// Midi channels
#define MIDI_CHANNEL_MONO 1
#define MIDI_CHANNEL_DUAL 2
#define MIDI_CHANNEL_RAW 3

// config button setup
#define BUTTON_PIN 2
