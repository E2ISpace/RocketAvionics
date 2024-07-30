#include <SPI.h>
#include <LoRa.h>
#include <Servo.h>

const int csPin = 10;    // LoRa 모듈 CS 핀
const int resetPin = 9;  // LoRa 모듈 리셋 핀
const int irqPin = 2;    // LoRa 모듈 IRQ 핀

Servo servo;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  // LoRa 모듈 초기화
  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(9209E5)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.onReceive(onReceive);
  LoRa.receive();


  //로켓 사출
  servo.attach(22); // 서보 모터 핀에 연결
  servo.write(0); // 서보 모터 초기 조건

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

  if (incoming == "REBOOT" || incoming == "R") {
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
  if else(incoming == "sos" || incoming == "SOS") {
    Serial.println("Parachute Deployed");
    servo.write(72); // 서보 모터를 72도 위치로 이동
    delay(1000); // 1초 대기
    servo.write(0); // 서보 모터를 원래 위치로 복귀
  }
}



