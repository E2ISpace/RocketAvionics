#ifndef LORA_GROUND_H
#define LORA_GROUND_H

#include <SPI.h>
#include <LoRa.h>

class LoRa_ground {
  private:
    const int csPin;
    const int resetPin;
    const int irqPin;

  public:
    LoRa_ground(int cs, int reset, int irq);

    void begin(long frequency);
    void sendMessage(const String &message);
    String receiveMessage();
    void processSerialInput();
};

#endif // LORA_GROUND_H
