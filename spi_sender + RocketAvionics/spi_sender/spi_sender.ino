#include <SPI.h>
#include <LoRa.h>
#include "RocketAvionics.h" // 헤더 파일 포함
int counter = 0;

void setup() {
  Serial.begin(9600); 
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(9209E5)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  setupRocketAvionics(); // Avionics 설정 함수 호출
}

void loop() {
  updateRocketAvionics(); // Avionics 데이터 업데이트
  
  Serial.print("Sending packet: ");
  Serial.println(counter);

 void onReceive(int packetSize) {
  if (packetSize == 0) return; // 패킷이 없으면 반환

  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  Serial.print("Received: ");
  Serial.println(incoming);

  if (incoming == "REBOOT") {
    Serial.println("Rebooting...");

    // 송신
    LoRa.beginPacket();
    LoRa.print("REBOOT_SUCCESS");
    LoRa.endPacket();
    delay(1000); // 송신 완료를 위한 지연 시간

    // 재부팅
    void(* resetFunc) (void) = 0; // 함수 포인터 정의
    resetFunc(); // 재부팅 실행
    
  }
}


// 패킷 전송
    LoRa.beginPacket();
    LoRa.print("x: ");
    LoRa.print(x);
    LoRa.print(", y: ");
    LoRa.print(y);
    LoRa.print(", z: ");
    LoRa.print(z);
    LoRa.print(", counter: ");
    LoRa.print(counter);
    LoRa.endPacket();

    counter;
    delay(10); // 필요한 대로 지연 시간 조정
}
