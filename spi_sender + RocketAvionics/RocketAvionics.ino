/*#include <Servo.h>*/
#include "MPU9250.h"
#include "RocketAvionics.h"
#include <math.h>

MPU9250 mpu; 

float x, y, z; // 공유 변수 정의
/*Servo servo;*/ 
bool state = 0;
double settingtime = 0; // 자동 사출을 위한 세팅 후 동작시간 변수선언

void setupRocketAvionics() {
    Wire.begin();
    Serial.begin(115200);

    
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
        Serial.println("MPU9250 setup failed");
        while (1);
    }

    mpu.verbose(true);
    mpu.setAccBias(-946.44, 633.1475, -1104.61);
    mpu.setGyroBias(280.12, -488.685, 167.025);
    mpu.calibrateMag();
    mpu.selectFilter(QuatFilterSel::MAHONY);

    delay(5000);
}

void updateRocketAvionics() {
    if (mpu.update()) {
        float yaw = mpu.getYaw();
        float pitch = mpu.getPitch();
        float roll = mpu.getRoll();

        float yawRad = yaw * DEG_TO_RAD;
        float pitchRad = pitch * DEG_TO_RAD;
        float rollRad = roll * DEG_TO_RAD;

        x = cos(pitchRad) * cos(yawRad);
        y = cos(pitchRad) * sin(yawRad);
        z = sin(pitchRad);

        Serial.print("Yaw: ");
        Serial.print(yaw);
        Serial.print(", Pitch: ");
        Serial.print(pitch);
        Serial.print(", Roll: ");
        Serial.print(roll);
        Serial.print(", x: ");
        Serial.print(x);
        Serial.print(", y: ");
        Serial.print(y);
        Serial.print(", z: ");
        Serial.print(z);
        Serial.print(" -> Angle: ");
        Serial.println(acos((x * 0 + y * 0 + z * 1) / (sqrt(x * x + y * y + z * z) * 1)) * RAD_TO_DEG);
    }
}
