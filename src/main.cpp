#include "main.h"

#include <MIDI.h>
#include <Bounce2.h>

#include "config.h"
#include "scales.h"

// Midi interface
#ifndef DEBUG
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

// Pins of all sensors
int sensorPins[] = SENSOR_PINS;

// Array to hold all the ldr sensor data using the bounce library
Bounce sensors[NUM_SENSORS];

// Button
Bounce button;

// Position Events
#define EV_NONE 0
#define EV_ON 1
#define EV_OFF 2

// Laser Nodes
laser_node ln_mono;


// Layout and scale
int curr_layout = 1;
int curr_scale = 0;



void setup() {
#ifdef DEBUG
    // Init the serial debug communication
    Serial.begin(115200);
#else
    // Init the midi interface
    MIDI.begin(MIDI_CHANNEL_OMNI);

#ifdef MIDI_CUSTOM_BAUD_RATE
    Serial.begin(115200);
#endif
#endif

    // Create the ldr sensors
    for (int i = 0; i < NUM_SENSORS; i++) {
        pinMode(sensorPins[i], INPUT);
        sensors[i].attach(sensorPins[i]);
        sensors[i].interval(SENSOR_DEBOUNCE);
    }

    // setup the laser nodes
    laser_node_init(&ln_mono);

    // Create the button debouncer
    pinMode(BUTTON_PIN, OUTPUT);
    button.attach(BUTTON_PIN);
    button.interval(10);

#ifdef DEBUG
    Serial.println("Hello from Laser Harp!");
#endif
}

void loop() {
    // check x sensors
    for (int x = 0; x < HARP_SIZE_X; x++) {
        sensors[x].update();

        if (sensors[x].fell()) {
            do_fell_x(&ln_mono, x, MIDI_CHANNEL_MONO);
        }
    }

    for (int x = 0; x < HARP_SIZE_X; x++) {
        if (sensors[x].read()) {
            do_read_x(&ln_mono, x, MIDI_CHANNEL_MONO);
        }
    }

    // check y sensors
    for (int y = 0; y < HARP_SIZE_Y; y++) {
        sensors[y + HARP_SIZE_X].update();

        if (sensors[y + HARP_SIZE_X].fell()) {
            do_fell_y(&ln_mono, y, MIDI_CHANNEL_MONO);
        }
    }

    for (int y = 0; y < HARP_SIZE_Y; y++) {
        if (sensors[y + HARP_SIZE_Y].read()) {
            do_read_y(&ln_mono, y, MIDI_CHANNEL_MONO);
        }
    }

    // check button
    button.update();
    if (button.rose()) {
#ifdef DEBUG
        Serial.println("CONFIG BUTTON!");
#endif
    }
}

void laser_node_init(laser_node *n) {
    n->x = -1;
    n->y = -1;
    n->xr = -1;
    n->yr = -1;
}

void do_fell_x(laser_node *n, int x, int midiChannel) {
    if (n->x == -1) return;

    n->xr = x;
    if (n->yr != -1) {
        midiPosOff(n->xr, n->yr, midiChannel);
    }
    n->x = -1;
}

void do_fell_y(laser_node *n, int y, int midiChannel) {
    if (n->y == -1) return;

    n->yr = y;
    if (n->xr != -1) {
        midiPosOff(n->xr, n->yr, midiChannel);
    }
    n->y = -1;
}

void do_read_x(laser_node *n, int x, int midiChannel) {
    if (n->x != -1) return;

    n->x = x;
    if (n->y != -1) midiPosOn(n->x, n->y, midiChannel);
}

void do_read_y(laser_node *n, int y, int midiChannel) {
    if (n->y != -1) return;

    n->y = y;
    if (n->x != -1) midiPosOn(n->x, n->y, midiChannel);
}

/*void loop() {
    // Iterate through every sensor on the x side
    for (int x = 0; x < HARP_SIZE_X; x++) {
        // Update the sensor
        sensors[x].update();
        // Set positions on rising edge
        if (sensors[x].rose()) {
            // TODO: Trigger a raw note on
            setIfUnset(&posMonoX, x);
            if (x < HARP_SIZE_X / 2) setIfUnset(&posDual1X, x);
            else setIfUnset(&posDual2X, x);
        }
        // Unset positions on falling edge
        if (sensors[x].fell()) {
            // TODO: Trigger a raw note off
            unsetIfEqual(&posMonoX, x);
            if (x < HARP_SIZE_X / 2) unsetIfEqual(&posDual1X, x);
            else unsetIfEqual(&posDual2X, x);
        }
    }
    // Iterate through every sensor on the y side
    for (int y = 0; y < HARP_SIZE_Y; y++) {
        // Update the sensor
        sensors[y + HARP_SIZE_X].update();
        // Set positions on rising edge
        if (sensors[y + HARP_SIZE_X].rose()) {
            // TODO: Trigger a raw note on
            setIfUnset(&posMonoY, y);
            if (y < HARP_SIZE_Y / 2) setIfUnset(&posDual1Y, y);
            else setIfUnset(&posDual2Y, y);
        }
        // Unset positions on falling edge
        if (sensors[y + HARP_SIZE_X].fell()) {
            // TODO: Trigger a raw note off
            unsetIfEqual(&posMonoY, y);
            if (y < HARP_SIZE_Y / 2) unsetIfEqual(&posDual1Y, y);
            else unsetIfEqual(&posDual2Y, y);
        }
    }
    // Read midi input
    #ifndef DEBUG
        MIDI.read(); // TODO: Neopixels?
    #endif
    // Delay for serial debugging
    #ifdef DEBUG_DELAY
        delay(DEBUG_DELAY);
    #endif
    }
    void setIfUnset(int *pos, int val) {
    // If the first laser beam gets interrupted, pos will be set to its location.
    // If the user (accidentally) interrupts another beam, we don't want to change
    // the position.
    if (*pos == -1) *pos = val;
    Serial.println(*pos);
    }
    void unsetIfEqual(int *pos, int val) {
    // If a laser beam got interrupted, pos is now set to its location.
    // If the user (accidentally) interrupts and releases another laser beam, we
    // don't want to unset the initial position.
    if (*pos == val) *pos = -1;
    }*/

void midiPosOn(int x, int y, int channel) {
    #ifdef DEBUG
        Serial.print("NOTE ON: ");
        Serial.print(x);
        Serial.print(", ");
        Serial.print(y);
        Serial.print(" -> note ");
        Serial.print(posToNote(x, y));
        Serial.print(", channel ");
        Serial.println(channel);
    #else
        MIDI.sendNoteOn(posToNote(x, y), 127, channel);
    #endif
}

void midiPosOff(int x, int y, int channel) {
    #ifdef DEBUG
        Serial.print("NOTE OFF: ");
        Serial.print(x);
        Serial.print(", ");
        Serial.print(y);
        Serial.print(" -> note ");
        Serial.print(posToNote(x, y));
        Serial.print(", channel ");
        Serial.println(channel);
    #else
        MIDI.sendNoteOn(posToNote(x, y), 0, channel);
    #endif
}

int posToNote(int x, int y) {
    return apply_scale(curr_scale, apply_layout(curr_layout, x, y));
}
