#include "LoRa_ground.h"

// Constructor implementation
LoRa_ground::LoRa_ground(int cs, int reset, int irq)
  : csPin(cs), resetPin(reset), irqPin(irq) {}

void LoRa_ground::begin(long frequency) {
  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(frequency)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void LoRa_ground::sendMessage(const String &message) {
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
}

String LoRa_ground::receiveMessage() {
  String received = "";
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }
  }
  return received;
}

void LoRa_ground::processSerialInput() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.equals("reboot") || input.equals("r")) {
      Serial.println("Sending reboot signal...");
      sendMessage("REBOOT");
    } 
    else if (input.equals("sos")) {
      Serial.println("Sending SOS signal...");
      sendMessage("SOS");
    }
    else {
      Serial.println("Unknown command.");
    }
  }
}
