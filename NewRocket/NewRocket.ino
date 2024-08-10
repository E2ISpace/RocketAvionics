#include <Servo.h>
#include "MPU.h"
#include <SD.h>
#include <SPI.h>
#include <MsTimer2.h>

#define RAD_TO_DEG 57.29577951308232

// 데이터 타입을 효율적으로 사용
const byte servoPin = 3;
const byte chipSelect = 53;

MPU9250 mpu;
Servo servo;
File dataFile;
float yaw, pitch, roll;
float angle = 0.0;            // 전역 변수로 선언하여 AngleCalculator에서 사용
unsigned long prevTime;       // 타이머를 위한 변수
unsigned long loopStartTime;  // 타이머를 위한 변수
float timeWhen;               // 타이머를 위한 변수
const unsigned long dt = 173; // 0.173 s=1730ms
float acc_Pitch, gyro_Pitch = 0;
float acc_Roll, gyro_Roll = 0;
float gyro_Yaw = 0;
float AccX, AccY, AccZ;
// float Acc[3]={0.0f};
float GyroX, GyroY, GyroZ;
// float Gyro[3]={0.0f};
float filter_Pitch, filter_Roll;
/*-------------------------------------*/
float rocketVector[3] = {0.0f};
float z_Unitvector[3] = {0.0f, 0.0f, 1.0f};
float a, b, c = 0.0f;
float SEq_1 = 1.0f, SEq_2 = 0.0f, SEq_3 = 0.0f, SEq_4 = 0.0f;
float rocketAngle = 0.0f;
float linearAcc[3];

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
        dataFile.println("Time:AccX,AccY,AccZ,GyroX,GyroY,GyroZ,Angle,linearAccX,linearAccY,linearAccZ");
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
    mpu.verbose(true);
    // mpu.calibrateMag();
    mpu.setAccBias(-946.44, 633.1475, -1104.61);
    mpu.setGyroBias(280.12, -488.685, 167.025);
    mpu.setMagBias(-261.46, -127.51, 394.03);
    mpu.setMagScale(1.32, 0.91, 0.87);
    mpu.selectFilter(QuatFilterSel::MAHONY);
    // startTime = millis(); // 설정 시작 시간을 기록
    // isAngleCheckEnabled = false; // 설정 완료 전까지 angle 체크 비활성화

    MsTimer2::set(13000, deployParachute); // time out 사출조건 - 1
    MsTimer2::start();
}

void loop()
{
    loopStartTime = millis();
    // if (loopStartTime - prevTime >= dt)
    // {
    if (mpu.update())
    {
        /*---------------------*/
        yaw = mpu.getYaw();
        pitch = mpu.getPitch();
        roll = mpu.getRoll();
        AccX = mpu.getAccX();
        AccY = mpu.getAccY();
        AccZ = mpu.getAccZ();
        GyroX = mpu.getGyroX();
        GyroY = mpu.getGyroY();
        GyroZ = mpu.getGyroZ(); // data Value 추가하기
        SEq_1 = mpu.getQuaternionW();
        SEq_2 = mpu.getQuaternionX();
        SEq_3 = mpu.getQuaternionY();
        SEq_4 = mpu.getQuaternionZ();
        linearAcc[0] = mpu.getLinearAcc(0);
        linearAcc[1] = mpu.getLinearAcc(1);
        linearAcc[2] = mpu.getLinearAcc(2);
        /*-------------------------------*/
        timeWhen = millis() / 1000.0f; // 시간 저장하기
        // filter(AccX, AccY, AccZ, GyroX, GyroY, GyroZ);
        rocketVector[0] = 2.0 * (SEq_2 * SEq_4 - SEq_1 * SEq_3);       // x축
        rocketVector[1] = 2.0 * (SEq_1 * SEq_2 + SEq_3 * SEq_4);       // y component
        rocketVector[2] = 1.0 - 2.0 * (SEq_2 * SEq_2 + SEq_3 * SEq_3); // z component
        a = rocketVector[0] * z_Unitvector[0] + rocketVector[1] * z_Unitvector[1] + rocketVector[2] * z_Unitvector[2];
        b = sqrt(rocketVector[0] * rocketVector[0] + rocketVector[1] * rocketVector[1] + rocketVector[2] * rocketVector[2]);
        c = sqrt(z_Unitvector[0] * z_Unitvector[0] + z_Unitvector[1] * z_Unitvector[1] + z_Unitvector[2] * z_Unitvector[2]);

        rocketAngle = acos(a / (b * c)) * RAD_TO_DEG; // inner product(dot product) Rocket vector with Z unit vector

        // Serial.println(rocketAngle); // test complete.
        float temp = timeWhen - 1.59f;
        if (dataFile)
        {
            char buffer[200];
            sprintf(buffer,
                    "%.2f, %.2f ,%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f",
                    temp, AccX, AccY, AccZ, GyroX, GyroY, GyroZ, rocketAngle, linearAcc[0], linearAcc[1], linearAcc[2]);
            dataFile.println(buffer);
            // dataFile.print(timeWhen - 1.59f);
            // dataFile.print(": ");
            // dataFile.print(AccX);
            // dataFile.print(", ");
            // dataFile.print(AccY);
            // dataFile.print(", ");
            // dataFile.print(AccZ);
            // dataFile.print(", ");
            // dataFile.print(GyroX);
            // dataFile.print(", ");
            // dataFile.print(GyroY);
            // dataFile.print(", ");
            // dataFile.print(GyroZ);
            // dataFile.print(", ");
            // dataFile.print(rocketAngle);
            // dataFile.print(", ");
            // dataFile.print(linearAcc[0]);
            // dataFile.print(", ");
            // dataFile.print(linearAcc[1]);
            // dataFile.print(", ");
            // dataFile.println(linearAcc[2]);
            dataFile.flush();
        }
    }
    prevTime = loopStartTime;
    // }
    Serial.println(millis() - loopStartTime); // time testing.
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

// void filter(float AccX, float AccY, float AccZ, float GyroX, float GyroY, float GyroZ)
// {
//     acc_Pitch = atan2(AccX, sqrt(AccY * AccY + AccZ * AccZ));
//     acc_Roll = atan2(AccY, sqrt(AccX * AccX + AccZ * AccZ));
//     gyro_Pitch = gyro_Pitch + GyroX * dt / 1000.0f;
//     gyro_Roll = gyro_Roll + GyroX * dt / 1000.0f;
//     gyro_Yaw = gyro_Yaw + GyroZ * dt / 1000.0f;
//     filter_Pitch = 0.98 * (filter_Pitch + gyro_Pitch) + 0.02 * acc_Pitch;
//     filter_Roll = 0.98 * (filter_Roll + gyro_Roll) + 0.02 * acc_Roll;
// }
