#include <SD.h>
#include <SPI.h>

const int chipSelect = 0; 

void setup() {
  // 시리얼 통신 시작
  Serial.begin(9600);
 
  // 테스트 파일 생성 및 작성
  File testFile = SD.open("test.txt", FILE_WRITE);
  
  if (testFile) {
    Serial.print("Writing to test.txt...");
    testFile.println("Testing 1, 2, 3...");
    testFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening test.txt");
  }

  // 파일 읽기 테스트
  testFile = SD.open("test.txt");
  if (testFile) {
    Serial.println("test.txt:");

    // 파일의 모든 내용을 읽어 시리얼 모니터에 출력합니다.
    while (testFile.available()) {
      Serial.write(testFile.read());
    }
    testFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // 루프에서 별도의 동작은 하지 않습니다.
}
