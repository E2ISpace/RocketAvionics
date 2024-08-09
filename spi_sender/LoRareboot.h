#ifndef LORAREBOOT_H
#define LORAREBOOT_H

#include <SPI.h>
#include <LoRa.h>

class LoRaReboot {
public:
  LoRaReboot(int csPin, int resetPin, int irqPin);
  void begin(long frequency);
  void onReceive(int packetSize);

private:
  int _csPin;
  int _resetPin;
  int _irqPin;

  static void _onReceiveWrapper(int packetSize);
};

#endif
