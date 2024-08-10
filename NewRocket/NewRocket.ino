#include <Servo.h>
#include "MPU.h"
#include <SD.h>
#include <SPI.h>

// 데이터 타입을 효율적으로 사용
const byte servoPin = 3;
const byte chipSelect = 10;

MPU9250 mpu;
Servo servo;
File dataFile;

float angle = 0.0; // 전역 변수로 선언하여 AngleCalculator에서 사용

void setup() {
    Serial.begin(9600);
    Wire.begin();
    servo.attach(servoPin);
    servo.write(0);

    if (!SD.begin(chipSelect)) {
        Serial.println(F("SD 카드 초기화 실패!"));
        while (1);
    }
    Serial.println(F("SD 카드 초기화 완료."));

    dataFile = SD.open("data.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.println(F("AccX,AccY,AccZ,Yaw,Pitch,Roll,Angle"));
    } else {
        Serial.println(F("데이터 파일 열기 오류"));
        while (1);
    }

    // MPU9250 초기 설정 최소화
    MPU9250Setting setting;
    setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
    setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;
    setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;

    if (!mpu.setup(0x68, setting)) {
        Serial.println(F("MPU9250 setup failed"));
        while (1);
    }

    // mpu.verbose(false); // 디버그 모드 비활성화
    mpu.setAccBias(-946.44, 633.1475, -1104.61);
    mpu.setGyroBias(280.12, -488.685, 167.025);
    mpu.calibrateMag();
    mpu.selectFilter(QuatFilterSel::MAHONY);
}

void loop() {
    if (mpu.update()) {
        float yaw = mpu.getYaw();
        float pitch = mpu.getPitch();
        float roll = mpu.getRoll();

        // 가속도계를 사용하는 계산은 제거
        AngleCalculator(yaw, pitch);

        // 데이터 로그
        logData(mpu.getAccX(), mpu.getAccY(), mpu.getAccZ(), yaw, pitch, roll, angle);

        Serial.print(F("Angle: "));
        Serial.println(angle);

        if (angle <= 0 || angle >= 180) {
            deployParachute();
        }
    }
}

void deployParachute() {
    Serial.println(F("Parachute Deployed"));
    servo.write(72);
    delay(1000);
    servo.write(0);
}

// 작은 함수는 인라인화하여 처리
inline void AngleCalculator(float yaw, float pitch) {
    float yawRad = yaw * DEG_TO_RAD;
    float pitchRad = pitch * DEG_TO_RAD;

    float dotProduct = sin(pitchRad);

    angle = acos(dotProduct) * RAD_TO_DEG;
}

// 데이터 로깅 함수
void logData(float AccX, float AccY, float AccZ, float yaw, float pitch, float roll, float angle) {
    if (dataFile) {
        dataFile.print(AccX);
        dataFile.print(F(","));
        dataFile.print(AccY);
        dataFile.print(F(","));
        dataFile.print(AccZ);
        dataFile.print(F(","));
        dataFile.print(yaw);
        dataFile.print(F(","));
        dataFile.print(pitch);
        dataFile.print(F(","));
        dataFile.print(roll);
        dataFile.print(F(","));
        dataFile.println(angle);
    }
}

