#include <SPI.h>
#include <LoRa.h>

const int csPin = 10;    // LoRa 모듈 CS 핀
const int resetPin = 9;  // LoRa 모듈 리셋 핀
const int irqPin = 2;    // LoRa 모듈 IRQ 핀

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  // LoRa 모듈 초기화
  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(9209E5)) { // 주파수를 수신기와 동일하게 설정
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  if (Serial.available() > 0) { // 시리얼 입력이 있는지 확인
    String input = Serial.readStringUntil('\n'); // 개행 문자까지 문자열을 읽음
    input.trim(); // 입력 문자열의 앞뒤 공백 제거

    // 입력된 문자열이 "reboot"인지 확인
    if (input.equals("reboot")) {
      Serial.println("Sending reboot signal...");
      LoRa.beginPacket();
      LoRa.print("REBOOT");
      LoRa.endPacket();
    } 
    if else(input.equals("sos")) {
      Serial.println("Sending reboot signal...");
      LoRa.beginPacket();
      LoRa.print("sos");
      LoRa.endPacket();
    }
    else {
      Serial.println("Unknown command. Type 'reboot' to reboot Arduino.");
    }
  }
  delay(500); // 버튼 입력 확인 주기 (500ms)
}


