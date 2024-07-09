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
  if (!LoRa.begin(920E6)) { // 920.9 MHz -> 920E6 (920 MHz)
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // 메시지 수신 모드로 전환
  LoRa.onReceive(onReceive);
  LoRa.receive();
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
    } else {
      Serial.println("Unknown command. Type 'reboot' to reboot Arduino.");
    }
  }
  delay(500); // 버튼 입력 확인 주기 (500ms)
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;

  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  Serial.print("Received: ");
  Serial.println(incoming);

  // 필요시 추가 처리
}
