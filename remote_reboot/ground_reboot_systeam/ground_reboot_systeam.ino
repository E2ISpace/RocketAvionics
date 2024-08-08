#include <SPI.h>
#include <LoRa.h>

const int csPin = 10;    // LoRa 모듈 CS 핀
const int resetPin = 9;  // LoRa 모듈 리셋 핀
const int irqPin = 2;    // LoRa 모듈 IRQ 핀

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender/Receiver");

  // LoRa 모듈 초기화
  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(920E6)) { // 주파수를 송신기와 동일하게 설정
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
    else if (input.equals("sos")) {
      Serial.println("Sending SOS signal...");
      LoRa.beginPacket();
      LoRa.print("SOS");
      LoRa.endPacket();
    }
    else {
      Serial.println("Unknown command.");
    }
  }

  // 메시지 수신 확인
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // 패킷 수신
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }
    Serial.print("Received: ");
    Serial.println(received);
    
    // 수신된 메시지에 따른 동작
    if (received.equals("REBOOT_SUCCESS")) {
      Serial.println("REBOOT_SUCCESS.");
      // 재부팅 명령 처리 코드 추가 가능
    }
    else if (received.equals("Parachute Deployed")) {
      Serial.println("Parachute Deployed SUCCESS.");
      // SOS 명령 처리 코드 추가 가능
    }
  }

  delay(500); // 버튼 입력 확인 주기 (500ms)
}
