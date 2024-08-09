#include <SPI.h>
#include <LoRa.h>

// LoRa 핀 설정
const int csPin = 10;    // LoRa 모듈 CS 핀
const int resetPin = 9;  // LoRa 모듈 리셋 핀
const int irqPin = 2;    // LoRa 모듈 IRQ 핀

// 모드 선택
bool isSender = false;    // 기본 모드는 수신기 모드

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(9209E5)) { // 주파수를 송신기와 동일하게 설정
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("LoRa Receiver Mode");
}

void loop() {
  // 시리얼 입력을 통한 모드 변경
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.equals("sender")) {
      isSender = true;
      Serial.println("Switched to LoRa Sender Mode");
    } else if (input.equals("receiver")) {
      isSender = false;
      Serial.println("Switched to LoRa Receiver Mode");
    }
  }

  if (isSender) {
    senderMode();
  } else {
    receiverMode();
  }
 // 주기
}

void senderMode() {
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
    else if (input.equals("sos")) {
      Serial.println("Sending SOS signal...");
      LoRa.beginPacket();
      LoRa.print("SOS");
      LoRa.endPacket();
    }
    else {
      Serial.println("Unknown command. Type 'reboot' to reboot Arduino or 'sos' for emergency.");
    }
  }
}

void receiverMode() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String receivedData = "";
    Serial.print("Received packet \n'");

    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    Serial.print(receivedData);
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

    // 데이터 분리 및 전송
    receivedData.trim(); // 문자열 앞뒤의 공백 제거
    float values[3]; // yaw, pitch, roll 세 종류의 데이터를 저장할 배열
    int index = 0; // 배열 인덱스
    
    // strtok 함수를 사용하여 콤마를 기준으로 문자열 분리
    char *ptr = strtok(const_cast<char*>(receivedData.c_str()), ",");
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
