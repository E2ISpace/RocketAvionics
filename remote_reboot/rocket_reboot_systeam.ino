#include <SPI.h>
#include <LoRa.h>

const int csPin = 10;    // LoRa module CS pin
const int resetPin = 9;  // LoRa module reset pin
const int irqPin = 2;    // LoRa module IRQ pin

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  // Initialize LoRa module
  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.onReceive(onReceive);
  LoRa.receive();
}

void loop() {
  // Do nothing, wait for LoRa messages
}

//수신
void onReceive(int packetSize) {
  if (packetSize == 0) return; // if there's no packet, return

  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  Serial.print("Received: ");
  Serial.println(incoming);

  if (incoming == "REBOOT") {
    Serial.println("Rebooting...");
    delay(1000); // Give time to print message
    asm volatile ("  jmp 0"); //reboot

    //송신
    LoRa.beginPacket();
      LoRa.print("REBOOT_SUCCES");
    LoRa.endPacket();
  }
}

