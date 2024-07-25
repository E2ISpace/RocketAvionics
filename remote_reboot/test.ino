// main.ino
#include <Arduino.h>
#include "LoRareboot.h"

const int csPin = 10;    // LoRa 모듈 CS 핀
const int resetPin = 9;  // LoRa 모듈 리셋 핀
const int irqPin = 2;    // LoRa 모듈 IRQ 핀
const long frequency = 9209E5; // 주파수 설정

LoRareboot receiver(csPin, resetPin, irqPin);

void setup() {
    receiver.begin(frequency);
}

void loop() {
    // 아무것도 하지 않고 LoRa 메시지를 기다림
}
