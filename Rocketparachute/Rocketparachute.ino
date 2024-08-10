#include "parachute.h"

Parachute parachute;

void setup() {
    parachute.begin();
}

void loop() {
    parachute.update();
}
