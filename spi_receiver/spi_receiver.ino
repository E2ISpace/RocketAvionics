#include <SPI.h>
#include <LoRa.h>



void setup() {
  
  public bool received_remote_reset = false;
  char remote_reset_key[8] = "RESET_KEY"


  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(9209E5)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();

  if (packetSize) {

    //받은 데이터 buff에 저장
    char buff[packetSize];
    Lora.readBytes(buff, packetSize);
    String data = buff;

    //리셋키 데이터를 받으면 실행
    if(data == "RESET_KEY") {
       Serial.print("Received Reset_Key.");
       received_remote_reset = true;
    }


    Serial.print("Received packet \n'");
    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
