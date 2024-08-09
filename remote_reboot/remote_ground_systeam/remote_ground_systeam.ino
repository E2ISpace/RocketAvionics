#include <SPI.h>
#include <LoRa.h>
#include "LoRa_ground.h"

// 핀 설정 (전역 변수로 정의)
const int csPin = 10;
const int resetPin = 9;
const int irqPin = 2;

// LoRa_ground 객체 생성
LoRa_ground lora(csPin, resetPin, irqPin);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender/Receiver");

  lora.begin(9209E5); // 주파수 설정
}

void loop() {
  lora.processSerialInput(); // 시리얼 입력 처리

  String received = lora.receiveMessage();
  if (received.length() > 0) {
    Serial.print("Received: ");
    Serial.println(received);

    if (received.equals("REBOOT_SUCCESS")) {
      Serial.println("REBOOT_SUCCESS.");
      // 재부팅 명령 처리 코드 추가 가능
    }
    else if (received.equals("Parachute Deployed")) {
      Serial.println("Parachute Deployed SUCCESS.");
      // SOS 명령 처리 코드 추가 가능
    }
  }

  delay(500);
}
