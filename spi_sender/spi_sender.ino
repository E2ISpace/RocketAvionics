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

  // //receiver에서 remote_reset_key를 받으면 
  // if(received_remote_reset == true) {
  //     LoRa.beginPacket();
  //       LoRa.print("Received Reset_Key.");
  //       LoRa.print("RESET_START.....");
  //     LoRa.endPacket();
  //     received_remote_reset = false;
  // }

  // send packet
  LoRa.beginPacket();
//  LoRa.print("10123456789,0123456789,0123456789,0123456789,0123456789,0123456789,0123456789,0123456789,0123456789,0123456789,0123456789");
    LoRa.print("01234567890,01234567890,01234567890,01234567890,01234567890");
    LoRa.print(counter);
  LoRa.endPacket();


  delay(10);
}
