#include "LaserHarp.h"
#include "Arduino.h"
#include "notes.h"


LaserHarp::Scale_t LaserHarp::scale = {
    { -1, C5, Db5,A5, Bb5,Gb6,-1, D7 },
    { C4, Db4,D5, Eb5,B5, -1, G6, Ab6},
    { Gb3,D4, Eb4,E5, -1, C6, Db6,A6 },
    { G3, Ab3,E4, -1, F5, Gb5,D6, Eb6},
    { Db3,A3, -1, F4, Gb4,G5, Ab5,E6 },
    { D3, -1, Bb3,B3, G4, Ab4,A5, Bb5},
    { -1, Eb3,E3, C4, Db4,A4, Bb4,B5 },
    { Ab2,A2, F3, Gb3,D4, Eb4,B4, -1 }
};

LaserHarp::SensorArray_t LaserHarp::sensorsX = { 7, 6, 12, 10, 5, 8, 13, 11 };
LaserHarp::SensorArray_t LaserHarp::sensorsY = { 4, 18, 19, 15, 9, 14, 16, 17 };


LaserHarp::State::State() :
    x(-1),
    y(-1),
    active(false) {}

int LaserHarp::readSensors(LaserHarp::SensorArray_t sensors, int previous) {
    int i, current, count;

    // if the previously active index is still HIGH, prefer that one and return it
    // that way, if multiple sensors get activated, the first combination will remain active
    // until released
    if (previous != -1 && digitalRead(sensors[previous])) {
        return previous;
    }

    // search for the first active sensor's index
    for (i = 0, count = 0; i < HARP_SIZE; i++) {
        if (digitalRead(sensors[i])) {
            current = i;
            count++;
        }
    }

    // exactly one sensor must be active
    if (count != 1) return -1;

    return current;
}

LaserHarp::LaserHarp() {
    // clear the callbacks
    setNoteOnCallback(nullptr);
    setNoteOffCallback(nullptr);
}

void LaserHarp::begin() {
    // setup all sensors
    for (int i = 0; i < HARP_SIZE; i++) {
        pinMode(sensorsX[i], INPUT);
        pinMode(sensorsY[i], INPUT);
    }

    // clear all notes
    if (callbacks.noteOff) {
        for (uint8_t n = 0; n <= 127; n++) {
            callbacks.noteOff(n, 0, HARP_CHANNEL);
        }
    }
}

void LaserHarp::update() {
    unsigned int activationTime;
    int8_t note;
    uint8_t velocity;

    // get the current x and y position
    current.x = readSensors(sensorsX, previous.x);
    current.y = readSensors(sensorsY, previous.y);

    // return if nothing changed
    if (current.x == prevRaw.x && current.y == prevRaw.y) return;

    // debug the raw x and y values
    /* if (current.x != -1 && prevRaw.x == -1 && callbacks.noteOn) {
        callbacks.noteOn(current.x + 60, 127, HARP_CHANNEL + 1);
    }
    if (current.x == -1 && prevRaw.x != -1 && callbacks.noteOff) {
        callbacks.noteOff(prevRaw.x + 60, 127, HARP_CHANNEL + 1);
    }
    if (current.y != -1 && prevRaw.y == -1 && callbacks.noteOn) {
        callbacks.noteOn(current.y + 60, 127, HARP_CHANNEL + 2);
    }
    if (current.y == -1 && prevRaw.y != -1 && callbacks.noteOff) {
        callbacks.noteOff(prevRaw.y + 60, 127, HARP_CHANNEL + 2);
    } */
    prevRaw = current;

    // if only one sensor was active, store the timestamp and return
    if ((current.x == -1) != (current.y == -1)) {
        current.timestamp = millis();
        return;
    }

    // return if the position did not change
    if (current.x == previous.x && current.y == previous.y) return;

    // get the active note
    current.active = false;
    if (current.x != -1 && current.y != -1) {
        note = flipped
            ? scale[HARP_SIZE - 1 - current.x][HARP_SIZE - 1 - current.y]
            : scale[current.y][current.x];

        if (note != -1) {
            current.active = true;
            current.note = note;

            // calculate the velocity
            activationTime = millis() - current.timestamp;
            if (activationTime > HARP_MAX_ACTIVATION_TIME) {
                velocity = 1;
            } else {
                velocity = 127 - (activationTime * 126) / HARP_MAX_ACTIVATION_TIME;
            }
        }
    }

    // release previous note
    if (previous.active && callbacks.noteOff) {
        callbacks.noteOff(previous.note, 0, HARP_CHANNEL);
    }

    // press current note
    if (current.active && callbacks.noteOn) {
        callbacks.noteOn(current.note, velocity, HARP_CHANNEL);
    }

    // wait the minimum active duration
    if (current.active && !previous.active) {
        //delay(HARP_ACTIVE_DURATION);
    }

    previous = current;
}

void LaserHarp::flip() {
    flipped = !flipped;
}

void LaserHarp::setNoteOnCallback(LaserHarp::NoteOnCallback_t callback) {
    callbacks.noteOn = callback;
}

void LaserHarp::setNoteOffCallback(LaserHarp::NoteOffCallback_t callback) {
    callbacks.noteOff = callback;
}
