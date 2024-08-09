#include "LoRareboot.h"

const int csPin = 10;
const int resetPin = 9;
const int irqPin = 2;

LoRaReboot receiver(csPin, resetPin, irqPin);

void setup() {
  receiver.begin(9209E5);
}

void loop() {
  // Nothing to do here, everything is handled by the LoRaReboot class
}
