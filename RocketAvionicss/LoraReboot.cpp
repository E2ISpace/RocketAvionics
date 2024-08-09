#include "LoraReboot.h"
#include <Servo.h>
LoraReboot* instance = nullptr;

Servo servo;

LoraReboot::LoraReboot(int csPin, int resetPin, int irqPin)
  : _csPin(csPin), _resetPin(resetPin), _irqPin(irqPin) {
  instance = this;
}

void LoraReboot::begin(long frequency) {
  Serial.begin(9600);
  servo.attach(22);
  while (!Serial);

  Serial.println("LoRa Receiver");

  LoRa.setPins(_csPin, _resetPin, _irqPin);
  if (!LoRa.begin(frequency)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.onReceive(_onReceiveWrapper);
  LoRa.receive();
}

void LoraReboot::_onReceiveWrapper(int packetSize) {
  if (instance) {
    instance->onReceive(packetSize);
  }
}

void LoraReboot::onReceive(int packetSize) {
  if (packetSize == 0) return;

  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  Serial.print("Received: ");
  Serial.println(incoming);

  if (incoming == "REBOOT" || incoming == "R") {
    Serial.println("Rebooting...");

    LoRa.beginPacket();
    LoRa.print("REBOOT_SUCCESS");
    LoRa.endPacket();
    delay(100);

    void(* resetFunc) (void) = 0;
    resetFunc();
  }
  else if (incoming == "sos" || incoming == "SOS")
  {
    Serial.println("Parachute Deployed");
    servo.write(72); 
    delay(100); 
    servo.write(0); 
  }
}
