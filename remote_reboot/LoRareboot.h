// LoRareboot.h
#ifndef LORAREBOOT_H
#define LORAREBOOT_H

#include <Arduino.h>
#include <LoRa.h>

class LoRareboot {
public:
    LoRareboot(int csPin, int resetPin, int irqPin);
    void begin(long frequency);
    void setFrequency(long frequency);
    static void onReceive(int packetSize);

private:
    int csPin;
    int resetPin;
    int irqPin;
    long frequency;
};

#endif // LORAREBOOT_H
