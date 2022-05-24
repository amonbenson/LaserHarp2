#include "harp.h"


MIDI_CREATE_DEFAULT_INSTANCE();
static int sensors[NUM_SENSORS] = SENSOR_PINS;

typedef int8_t scale_t[HARP_SIZE][HARP_SIZE];
static scale_t scales[NUM_MIDI_SCALES] = MIDI_SCALES;
static int curr_scale_id;

static struct harp_state curr_state, prev_state;


static void harp_release_all() {
    for (uint8_t note = 0; note <= 127; note++) {
        MIDI.sendNoteOff(note, 0, MIDI_CHANNEL);
    }
}

void harp_begin() {
    // setup all sensors
    for (int i = 0; i < NUM_SENSORS; i++) {
        pinMode(sensors[i], INPUT);
    }

    // reset the harp state
    curr_scale_id = 0;
    curr_state = { -1, -1, false };
    prev_state = { -1, -1, false };

    // setup midi
    MIDI.begin(MIDI_CHANNEL_OMNI);

    harp_release_all();
}

static int get_active_index(int *sensors, int size, int prev_index) {
    int i, count, active_index;

    // if the previously active index is still HIGH, prefer that one and return it
    // that way, if multiple sensors get activated, the first combination will remain active
    // until released
    if (prev_index != -1 && digitalRead(sensors[prev_index])) {
        return prev_index;
    }

    // search for the first active sensor's index
    for (i = 0, count = 0; i < size; i++) {
        if (digitalRead(sensors[i])) {
            active_index = i;
            count++;
        }
    }

    // return the active index only if exactly one sensor was activated
    if (count == 1) return active_index;
    else return -1;
}

static int8_t get_scale_note(int x, int y) {
    return scales[curr_scale_id][y][x];
}

void harp_update() {
    int8_t signed_note;

    // return if the previous note's time did not elapse yet (TODO: handle millis() overflow)
    if (prev_state.active && millis() < prev_state.start_time + MIDI_DURATION) return;

    // get the current x and y position
    curr_state.sx = get_active_index(sensors, HARP_SIZE, prev_state.sx);
    curr_state.sy = get_active_index(sensors + HARP_SIZE, HARP_SIZE, prev_state.sy);

    // return if only one sensor is active. This will normally happen if a sensor is just beeing
    // pressed or released and helps prevent audio glitches when changing pitch
    if ((curr_state.sx == -1) != (curr_state.sy == -1)) return;

    // return if the position did not change
    if (curr_state.sx == prev_state.sx && curr_state.sy == prev_state.sy) return;

    // if the position is valid, check the scale note.
    curr_state.active = false;
    if (curr_state.sx != -1 && curr_state.sy != -1) {
        signed_note = get_scale_note(curr_state.sx, curr_state.sy);
    
        // if the scale note is valid too, set the current state to active
        if (signed_note != -1) {
            curr_state.note = signed_note;
            curr_state.active = true;
            curr_state.start_time = millis();
        }
    }

    // release old key
    if (prev_state.active) {
        #ifdef DEBUG
            Serial.print("up: ");
            Serial.print(prev_state.sx);
            Serial.print(", ");
            Serial.print(prev_state.sy);
            Serial.println();
        #else
            MIDI.sendNoteOn(prev_state.note, 0, MIDI_CHANNEL);
        #endif
    }

    // press new key
    if (curr_state.active) {
        #ifdef DEBUG
            Serial.print("down: ");
            Serial.print(curr_state.sx);
            Serial.print(", ");
            Serial.print(curr_state.sy);
            Serial.println();
        #else
            MIDI.sendNoteOn(curr_state.note, MIDI_VELOCITY, MIDI_CHANNEL);
        #endif
    }

    // update previous state
    prev_state = curr_state;
}

void harp_next_scale() {
    harp_release_all();

    curr_scale_id = (curr_scale_id + 1) % NUM_MIDI_SCALES;
}
