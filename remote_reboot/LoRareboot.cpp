// LoRareboot.cpp
#include "LoRareboot.h"

LoRareboot::LoRareboot(int csPin, int resetPin, int irqPin)
    : csPin(csPin), resetPin(resetPin), irqPin(irqPin), frequency(0) {}

void LoRareboot::begin(long frequency) {
    this->frequency = frequency;
    Serial.begin(9600);
    while (!Serial);

    Serial.println("LoRa Receiver");

    // LoRa 모듈 초기화
    LoRa.setPins(csPin, resetPin, irqPin);
    if (!LoRa.begin(frequency)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    LoRa.onReceive(onReceive);
    LoRa.receive();
}

void LoRareboot::setFrequency(long frequency) {
    this->frequency = frequency;
    if (!LoRa.begin(frequency)) {
        Serial.println("Setting frequency failed!");
        while (1);
    }
}

void LoRareboot::onReceive(int packetSize) {
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
        //delay(1000); // 송신 완료를 위한 지연 시간

        // 재부팅
        void(* resetFunc) (void) = 0; // 함수 포인터 정의
        resetFunc(); // 재부팅 실행
    }
}
