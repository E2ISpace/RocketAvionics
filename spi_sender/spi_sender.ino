#include <SPI.h>
#include <LoRa.h>
#include "RocketAvionics.h" // 헤더 파일 포함
#include "LoRareboot.h"
int counter = 0;

const int csPin = 10;    // LoRa 모듈 CS 핀
const int resetPin = 9;  // LoRa 모듈 리셋 핀
const int irqPin = 2;    // LoRa 모듈 IRQ 핀
const long frequency = 9209E5; // 주파수 설정

LoRaReboot receiver(csPin, resetPin, irqPin);

void setup() {
  receiver.begin(frequency);
  setupRocketAvionics(); // Avionics 설정 함수 호출
  
}

void loop() {
  updateRocketAvionics(); // Avionics 데이터 업데이트
  
  Serial.print("Sending packet: ");
  Serial.println(counter);

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
<<<<<<< HEAD
// void onReceive(int packetSize) {
//   if (packetSize == 0) return; // 패킷이 없으면 반환

//   String incoming = "";
//   while (LoRa.available()) {
//     incoming += (char)LoRa.read();
//   }

//   Serial.print("Received: ");
//   Serial.println(incoming);

//   if (incoming == "REBOOT") {
//     Serial.println("Rebooting...");

//     // 송신
//     LoRa.beginPacket();
//     LoRa.print("REBOOT_SUCCESS");
//     LoRa.endPacket();
//     delay(1000); // 송신 완료를 위한 지연 시간

//     Serial.println("REBOOT_SUCCESS");
//     // 재부팅
//     void(* resetFunc) (void) = 0; // 함수 포인터 정의
//     resetFunc(); // 재부팅 실행
    
//   }
// }
=======



>>>>>>> ccffc76d1ea85c5e3a8aba8beb54e3aa145a7bc9
