#include <Servo.h>
#include "MPU9250.h"
#include <math.h>

MPU9250 mpu;
Servo servo; // 서보 모터 제어를 위한 객체 생성

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    servo.attach(22); // 서보 모터 핀에 연결
    servo.write(0); // 서보 모터 초기 조건
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
        Serial.println("MPU9250 setup failed");
        while (1);
    }

    mpu.verbose(true);
    mpu.setAccBias(-946.44, 633.1475, -1104.61);
    mpu.setGyroBias(280.12, -488.685, 167.025);
    mpu.calibrateMag();
    mpu.selectFilter(QuatFilterSel::MAHONY);

}

void loop()
{
    if (mpu.update())
    {
        float yaw = mpu.getYaw();
        float pitch = mpu.getPitch();
        float roll = mpu.getRoll();

        // 도(degree)에서 라디안(radian)으로 변환
        float yawRad = yaw * DEG_TO_RAD;
        float pitchRad = pitch * DEG_TO_RAD;
        float rollRad = roll * DEG_TO_RAD;

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
        float angle = acos(dotProduct / vectorMagnitude) * RAD_TO_DEG;

        // 값 출력
        Serial.print("Yaw: ");
        Serial.print(yaw);
        Serial.print(", Pitch: ");
        Serial.print(pitch);
        Serial.print(", Roll: ");
        Serial.print(roll);
        Serial.print(" -> Angle: ");
        Serial.println(angle);
        Serial.print("");
    
        // 조건 확인 및 서보 모터 작동
        if (angle <= 60 || angle >= 110)
        {
            deployParachute();
        }

    }
}

void deployParachute()
{
    Serial.println("Parachute Deployed");
    servo.write(72); // 서보 모터를 72도 위치로 이동
    delay(1000); // 1초 대기
    servo.write(0); // 서보 모터를 원래 위치로 복귀
}