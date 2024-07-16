void setup() {
  Serial.begin(9600); // 시리얼 통신 시작
  while (!Serial) {
    ; // 시리얼 포트가 준비될 때까지 기다립니다.
  }
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // 개행 문자까지 문자열을 읽음
    input.trim(); // 문자열 앞뒤의 공백 제거

    // 콤마를 기준으로 문자열을 분리하여 배열에 저장
    float values[3]; // yaw, pitch, roll 세 종류의 데이터를 저장할 배열
    int index = 0; // 배열 인덱스
    
    // strtok 함수를 사용하여 콤마를 기준으로 문자열 분리
    char *ptr = strtok(const_cast<char*>(input.c_str()), ",");
    while (ptr != NULL && index < 3) {
      values[index++] = atof(ptr); // 문자열을 실수형(float)으로 변환하여 배열에 저장
      ptr = strtok(NULL, ",");
    }

    // 배열에 저장된 yaw, pitch, roll 값을 출력 또는 사용
    Serial.print("Yaw: ");
    Serial.println(values[0]);
    Serial.print("Pitch: ");
    Serial.println(values[1]);
    Serial.print("Roll: ");
    Serial.println(values[2]);
  }
}

