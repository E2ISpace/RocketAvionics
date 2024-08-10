#include <Servo.h>
#include "MPU.h"
#include <SD.h>
#include <SPI.h>

// 데이터 타입을 효율적으로 사용
const byte servoPin = 3;
const byte chipSelect = 53;

MPU9250 mpu;
Servo servo;
File dataFile;

float angle = 0.0; // 전역 변수로 선언하여 AngleCalculator에서 사용
unsigned long startTime; // 타이머를 위한 변수
unsigned long loopStartTime; // 타이머를 위한 변수
unsigned long timeWhen; // 타이머를 위한 변수
unsigned long dt; // 타이머를 위한 변수

bool isAngleCheckEnabled = false; // angle 체크 활성화 여부

void setup() {
    Serial.begin(9600);
    Wire.begin();
    servo.attach(servoPin);
    servo.write(0);
    delay(1000);

    if (!SD.begin(chipSelect)) {
      while (1); // 초기화 실패 시 무한 루프
    }
    
    dataFile = SD.open("data.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.println(F("Time,AccX,AccY,AccZ,GyroX,GyroY,GyroZ,Yaw,Pitch,Roll,Angle"));
    } else {
        while (1); // 파일 열기 실패 시 무한 루프
    }

    // MPU9250 초기 설정 최소화
    MPU9250Setting setting;
    setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
    setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;
    setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
    setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_200HZ;
    setting.gyro_fchoice = 0x03;
    setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_41HZ;
    setting.accel_fchoice = 0x01;
    setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;

    if (!mpu.setup(0x68, setting)) {
      while (1); // MPU9250 초기화 실패 시 무한 루프
    }

    mpu.setAccBias(-946.44, 633.1475, -1104.61);
    mpu.setGyroBias(280.12, -488.685, 167.025);

    startTime = millis(); // 설정 시작 시간을 기록
    isAngleCheckEnabled = false; // 설정 완료 전까지 angle 체크 비활성화
}

void loop() {
    loopStartTime=micros();
    if (mpu.update()) {
        float yaw = mpu.getYaw();
        float pitch = mpu.getPitch();
        float roll = mpu.getRoll();
        float AccX = mpu.getAccX();
        float AccY = mpu.getAccY();
        float AccZ = mpu.getAccZ(); //data Value 추가하기
        timeWhen = startTime-millis();

        // Angle계산
        AngleCalculator(yaw, pitch, roll);

        if (dataFile) {
            dataFile.print(AccX);
            dataFile.print(", ");
            dataFile.print(AccY);
            dataFile.print(", ");
            dataFile.print(AccZ);
            dataFile.print(", ");
            dataFile.print(yaw);
            dataFile.print(", ");
            dataFile.print(pitch);
            dataFile.print(", ");
            dataFile.print(roll);
            dataFile.print(", ");
            dataFile.println(angle);
            dataFile.flush(); // 데이터가 즉시 쓰이도록 보장 
        } 

        // 조건 확인 및 서보 모터 작동
        if (isAngleCheckEnabled && (angle <= 65 || angle >= 110)) {
            deployParachute();
        }
        else if (isAngleCheckEnabled && (AccZ <= abs(0.3))) {
            deployParachute();
        }
    }
    dt=loopStartTime-micros();
}

// 낙하산 사출하는 함수
void deployParachute() { 
    servo.write(72);
    delay(1000);
    servo.write(0);
}

// Angle계산하는 함수
void AngleCalculator(float yaw, float pitch, float roll) {
    // 도(degree)에서 라디안(radian)으로 변환
    float yawRad = yaw * DEG_TO_RAD;
    float pitchRad = pitch * DEG_TO_RAD;

    // 방향 벡터 계산 (yaw와 pitch 사용)
    float x = cos(pitchRad) * cos(yawRad);
    float y = cos(pitchRad) * sin(yawRad);
    float z = sin(pitchRad);

    // 목표 벡터 (위쪽 방향)
    float targetX = 0;
    float targetY = 0;
    float targetZ = 1;

    // 벡터 내적
    float dotProduct = x * targetX + y * targetY + z * targetZ;
    float vectorMagnitude = sqrt(x * x + y * y + z * z) * sqrt(targetX * targetX + targetY * targetY + targetZ * targetZ);

    // 사이각 계산
    angle = acos(dotProduct / vectorMagnitude) * RAD_TO_DEG;
}



// void complementary_Filter(){

// }