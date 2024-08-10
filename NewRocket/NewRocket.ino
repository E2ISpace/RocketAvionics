#include <Servo.h>
#include "MPU.h"
#include <SD.h>
#include <SPI.h>
#include <MsTimer2.h>

// 데이터 타입을 효율적으로 사용
const byte servoPin = 3;
const byte chipSelect = 53;

MPU9250 mpu;
Servo servo;
File dataFile;

float angle = 0.0;           // 전역 변수로 선언하여 AngleCalculator에서 사용
unsigned long prevTime;      // 타이머를 위한 변수
unsigned long loopStartTime; // 타이머를 위한 변수
float timeWhen;              // 타이머를 위한 변수
const unsigned long dt = 20; // 0.02 s=20ms
float acc_Pitch, gyro_Pitch = 0;
float acc_Roll, gyro_Roll = 0;
float gyro_Yaw = 0;
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float filter_Pitch, filter_Roll;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    servo.attach(servoPin);
    servo.write(0);
    delay(1000);

    if (!SD.begin(chipSelect))
    {
        while (1)
            ; // 초기화 실패 시 무한 루프
    }

    dataFile = SD.open("data.txt", FILE_WRITE);
    if (dataFile)
    {
        dataFile.println("Time,AccX,AccY,AccZ,GyroX,GyroY,GyroZ,Angle");
    }
    else
    {
        while (1)
            ; // 파일 열기 실패 시 무한 루프
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

    if (!mpu.setup(0x68, setting))
    {
        while (1)
            ; // MPU9250 초기화 실패 시 무한 루프
    }

    mpu.setAccBias(-946.44, 633.1475, -1104.61);
    mpu.setGyroBias(280.12, -488.685, 167.025);
    // startTime = millis(); // 설정 시작 시간을 기록
    // isAngleCheckEnabled = false; // 설정 완료 전까지 angle 체크 비활성화

    MsTimer2::set(13000, deployParachute);
    MsTimer2::start();
}

void loop()
{
    loopStartTime = millis();
    if (loopStartTime - prevTime >= dt)
    {
        if (mpu.update())
        {
            // yaw = mpu.getYaw();
            // pitch = mpu.getPitch();
            // roll = mpu.getRoll();
            AccX = mpu.getAccX();
            AccY = mpu.getAccY();
            AccZ = mpu.getAccZ();
            GyroX = mpu.getGyroX();
            GyroY = mpu.getGyroY();
            GyroZ = mpu.getGyroZ();        // data Value 추가하기
            timeWhen = millis() / 1000.0f; // 시간 저장하기
            filter(AccX, AccY, AccZ, GyroX, GyroY, GyroZ);

            // Angle계산
            // AngleCalculator(yaw, pitch, roll);

            if (dataFile)
            {
                dataFile.print(timeWhen - 1.59f);
                Serial.print("filter_Pitch:"); // check complete
                Serial.println(filter_Pitch); // check complete
                Serial.print("filter_Roll:");  // check complete
                Serial.println(filter_Roll);  // check complete
                dataFile.print(", ");
                dataFile.print(AccX);
                dataFile.print(", ");
                dataFile.print(AccY);
                dataFile.print(", ");
                dataFile.print(AccZ);
                dataFile.print(", ");
                dataFile.print(GyroX);
                dataFile.print(", ");
                dataFile.print(GyroY);
                dataFile.print(", ");
                dataFile.println(GyroZ);
                dataFile.flush();
                // dataFile.print(", ");
                // dataFile.println(angle);
            }

            // // 조건 확인 및 서보 모터 작동
            // if (angle <= 65 || angle >= 110)
            // {
            //     deployParachute();
            // }
            // else if (AccZ <= abs(0.3))
            // {
            //     deployParachute();
            // }
            // while(1){
            //     dt2 = millis()-loopStartTime;
            //     if(200>=dt2){
            //         Serial.print(dt2);
            //         break;
            //     }
            //     delay(10);
            // }
        }
        prevTime = loopStartTime;
    }
}

// 낙하산 사출하는 함수
void deployParachute()
{
    servo.write(72);
    delay(1000);
    servo.write(0);
    Serial.println("parachute timeout");
    MsTimer2::stop();
}

// Angle계산하는 함수
void AngleCalculator(float yaw, float pitch, float roll)
{
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

void filter(float AccX, float AccY, float AccZ, float GyroX, float GyroY, float GyroZ)
{
    acc_Pitch = atan2(AccX, sqrt(AccY*AccY + AccZ*AccZ));
    acc_Roll = atan2(AccY, sqrt(AccX*AccX + AccZ*AccZ));
    gyro_Pitch = gyro_Pitch + GyroX * dt/1000.0f;
    gyro_Roll = gyro_Roll + GyroX * dt/1000.0f;
    gyro_Yaw = gyro_Yaw + GyroZ * dt/1000.0f;
    filter_Pitch = 0.98 * (filter_Pitch + gyro_Pitch) + 0.02 * acc_Pitch;
    filter_Roll = 0.98 * (filter_Roll + gyro_Roll) + 0.02 * acc_Roll;
}