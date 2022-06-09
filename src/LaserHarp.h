#pragma once

#include <stdint.h>

#define HARP_SIZE 8
#define HARP_CHANNEL 1
#define HARP_MAX_ACTIVATION_TIME 35


class LaserHarp {
    public:
        typedef int8_t Scale_t[HARP_SIZE][HARP_SIZE];
        typedef uint8_t SensorArray_t[HARP_SIZE];

        typedef void (*NoteOnCallback_t)(uint8_t note, uint8_t velocity, uint8_t channel);
        typedef void (*NoteOffCallback_t)(uint8_t note, uint8_t velocity, uint8_t channel);

        struct State {
            State();

            int x;
            int y;

            bool active;
            uint8_t note;

            unsigned long timestamp;
        };

        LaserHarp();

        void begin();
        void update();
        void flip();

        void setNoteOnCallback(NoteOnCallback_t callback);
        void setNoteOffCallback(NoteOffCallback_t callback);
    
    private:
        static Scale_t scale;
        static SensorArray_t sensorsX, sensorsY;

        struct {
            NoteOnCallback_t noteOn;
            NoteOffCallback_t noteOff;
        } callbacks;

        State current; // active sensor state
        State previous; // previous note state
        State prevRaw; // previous (possibly incomplete) state
        bool flipped;

        int readSensors(SensorArray_t sensors, int previous);
};
