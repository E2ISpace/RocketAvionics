#include <SPI.h>
#include <LoRa.h>

const int csPin = 10;    // LoRa 모듈 CS 핀
const int resetPin = 9;  // LoRa 모듈 리셋 핀
const int irqPin = 2;    // LoRa 모듈 IRQ 핀

const int reset_buttonpin = 5; // 리셋 버튼 핀
int flag = 0; // 플래그 변수

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  // LoRa 모듈 초기화
  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  pinMode(reset_buttonpin, INPUT); // 리셋 버튼 핀을 입력 모드로 설정
}

void loop() {
  // 리셋 버튼이 눌렸을 때 "REBOOT" 메시지를 전송
  if (digitalRead(reset_buttonpin) == HIGH) {
    if (flag == 0) {
      flag = 1; // 플래그 설정
      Serial.println("Sending REBOOT...");

      LoRa.beginPacket();
      LoRa.print("REBOOT");
      LoRa.endPacket();
    }
  } else {
    flag = 0; // 버튼이 눌리지 않으면 플래그 초기화
  }

  delay(500); // 버튼 입력 확인 주기 (500ms)

  // 메시지 수신 모드로 전환
  LoRa.onReceive(onReceive);
  LoRa.receive();
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
