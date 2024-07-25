#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(9209E5)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
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

    // yaw, pitch, roll 값을 시리얼 포트로 전송
    Serial.print(values[0]);
    Serial.print(",");
    Serial.print(values[1]);
    Serial.print(",");
    Serial.println(values[2]);
  }
}

