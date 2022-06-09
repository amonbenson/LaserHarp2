#include <Arduino.h>
#include <MIDI.h>
#include "LaserHarp.h"
#include "config.h"


#define NOTE_ON 0x90
#define NOTE_OFF 0x80


MIDI_CREATE_DEFAULT_INSTANCE();
LaserHarp harp;


void noteOn(uint8_t note, uint8_t velocity, uint8_t channel) {
    MIDI.sendNoteOn(note, velocity, channel);
}

void noteOff(uint8_t note, uint8_t velocity, uint8_t channel) {
    MIDI.sendNoteOff(note, 0, channel);
}

void setup() {
    MIDI.begin();
    Serial.begin(115200);

    harp.setNoteOnCallback(noteOn);
    harp.setNoteOffCallback(noteOff);
    harp.begin();
}

void loop() {
    harp.update();
}
