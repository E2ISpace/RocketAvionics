#include <SPI.h>
#include <LoRa.h>
#include "LoRa_ground.h"

// LoRa 핀 설정
const int csPin = 10;    // LoRa 모듈 CS 핀
const int resetPin = 9;  // LoRa 모듈 리셋 핀
const int irqPin = 2;    // LoRa 모듈 IRQ 핀

LoRa_ground lora(csPin, resetPin, irqPin);
// 모드 선택
bool isSender = false;    // 기본 모드는 수신기 모드

void setup() {
  Serial.begin(9600);
  while (!Serial);

  lora.begin(9209E5);  // 주파수를 송신기와 동일하게 설정

  Serial.println("LoRa Receiver Mode");
}

void loop() {
  // 시리얼 입력을 통한 모드 변경
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.equals("se")) {
      isSender = true;
      Serial.println("Switched to LoRa Sender Mode");
    } else if (input.equals("re")) {
      isSender = false;
      Serial.println("Switched to LoRa Receiver Mode");
    }
  }

  if (isSender) {
    lora.processSerialInput();  // 송신 모드 처리
  } else {
    receiverMode();  // 수신 모드 처리
  }
}

void receiverMode() {
  String receivedMessage = lora.receiveMessage();
  
  if (receivedMessage.length() > 0) {
    Serial.print("Received packet \n'");
    Serial.print(receivedMessage);
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

    // 데이터 분리 및 전송
    float values[3] = {0.0, 0.0, 0.0}; // yaw, pitch, roll 세 종류의 데이터를 저장할 배열
    int index = 0; // 배열 인덱스 초기화

    char *ptr = strtok(receivedMessage.c_str(), ",");
    while (ptr != NULL && index < 3) {
      values[index++] = atof(ptr); // 문자열을 실수형(float)으로 변환하여 배열에 저장
      ptr = strtok(NULL, ",");
    }

    // 데이터 전송 (CSV 형식)
    Serial.print(values[0]);
    Serial.print(",");
    Serial.print(values[1]);
    Serial.print(",");
    Serial.println(values[2]);
  }
}
