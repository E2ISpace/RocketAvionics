#include <SD.h>
#include <SPI.h>

#define SS 10


void setup() {
  // 시리얼 통신 시작
  Serial.begin(9600);
  delay(1000); // 시리얼 통신 초기화 대기

  // SD 카드 초기화
  if (!SD.begin(SS)) {
    Serial.println("SD 카드 초기화 실패!");
    return;
  }
  Serial.println("SD 카드 초기화 완료.");

  // 테스트 파일 생성 및 작성
  File testFile = SD.open("test.txt", FILE_WRITE);

  if (testFile) {
    Serial.print("test.txt에 쓰기...");
    testFile.println("Testing 1, 2, 3...");
    testFile.close();
    Serial.println("완료.");
  } else {
    Serial.println("test.txt 파일 열기 오류");
  }

  // 파일 읽기 테스트
  testFile = SD.open("test.txt");
  if (testFile) {
    Serial.println("test.txt 내용:");

    // 파일의 모든 내용을 읽어 시리얼 모니터에 출력
    while (testFile.available()) {
      Serial.write(testFile.read());
    }
    testFile.close();
  } else {
    Serial.println("test.txt 파일 열기 오류");
  }
}

void loop() {
  // 루프에서 별도의 동작은 하지 않습니다.
}
