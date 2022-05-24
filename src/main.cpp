#include <Arduino.h>
#include <Bounce2.h>
#include "harp.h"


Bounce setup_button;


void setup() {
    harp_begin();

    #ifdef DEBUG
        Serial.begin(115200);
        Serial.println("Laser Harp V3 by Schlegel Flegel");
    #endif

    setup_button.attach(SETUP_PIN);
    setup_button.interval(10);
}

void loop() {
    harp_update();

    setup_button.update();
    if (setup_button.rose()) {
        harp_next_scale();
    }
}
