#include <SPI.h>
#include <LoRa.h>

int counter = 0;

void setup() {
  Serial.begin(9600); 
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(9209E5)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
//  LoRa.print("10123456789,0123456789,0123456789,0123456789,0123456789,0123456789,0123456789,0123456789,0123456789,0123456789,0123456789");
  LoRa.print("01234567890,01234567890,01234567890,01234567890,01234567890");
  LoRa.print(counter);
  LoRa.endPacket();


  delay(10);
}
