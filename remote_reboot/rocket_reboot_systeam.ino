#include <SPI.h>
#include <LoRa.h>

const int csPin = 10;    // LoRa 모듈 CS 핀
const int resetPin = 9;  // LoRa 모듈 리셋 핀
const int irqPin = 2;    // LoRa 모듈 IRQ 핀

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  // LoRa 모듈 초기화
  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.onReceive(onReceive);
  LoRa.receive();
}

void loop() {
  // 아무것도 하지 않고 LoRa 메시지를 기다림
}

// 수신 함수
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



