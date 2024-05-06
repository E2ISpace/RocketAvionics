#include "MPU9250.h"
#include "./algorithm/MadgwickAHRS.h"
#include <math.h>
// https://github.com/hideakitai/MPU9250


void setup()
{
    Serial.begin(115200);
    Wire.begin();
    delay(2000);
    MPU9250Setting setting;
    setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
    setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;
    setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
    setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_200HZ;
    setting.gyro_fchoice = 0x03;
    setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_41HZ;
    setting.accel_fchoice = 0x01;
    setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;

    mpu.setup(0x68, setting);

    // mpu.setup(0x68); // change to your own address
    Serial.println("Calibrate..");

    delay(100);

    // calibrate anytime you want to
    mpu.calibrateAccelGyro();
    mpu.setGyroBias(2.2930535, -3.65441, 1.27980);
    // 2.293053522	-3.654414456	1.279805581 = 5000 data average.
    mpu.calibrateMag();
    mpu.selectFilter(QuatFilterSel::MADGWICK);

    /*testing..*/
    // time_former = micros();
    // Quaternion qua;
}

void loop()
{
    mpu.update();
    /*---------저장 중-----------*/
    // mpu.update_accel_gyro_offset(); // update accel gyro
    Serial.print(mpu.getAccX());
    Serial.print(", ");
    Serial.print(mpu.getAccY());
    Serial.print(", ");
    Serial.println(mpu.getAccZ());
    Serial.print(mpu.getGyroX());
    Serial.print(", ");
    Serial.print(mpu.getGyroY());
    Serial.print(", ");
    Serial.println(mpu.getGyroZ());

    delay(100);
}
/*
//
/*-----------------testing---------------*/
// struct Quaternion
// {
//     float w, x, y, z;
// };
// struct EulerAngles
// {
//     float roll_e, pitch_e, yaw_e;
// };
// volatile float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};
// Quaternion qua;
// EulerAngles eul;

// MPU9250 mpu; // You can also use MPU9255 as is
// float time_now;
// float time_former;
// float deltat;
//main
/*
time_now = micros();
    deltat = (float)(time_now - time_former) / 1000000.0f;
    time_former = time_now;
    MadgwickQuaternionUpdate(mpu.getAccX(), mpu.getAccY(), mpu.getAccZ(),
                             mpu.getGyroX(), mpu.getGyroY(), mpu.getGyroZ(),
                             mpu.getMagX(), mpu.getMagY(), mpu.getMagY(), deltat);
    qua.w = q[0];
    qua.x = q[1];
    qua.y = q[2];
    qua.z = q[3];
    eul = ToEulerAngles(qua);
    eul.yaw_e += 0.8;
    if (eul.yaw_e > M_PI)
    {
        eul.yaw_e -= 2 * M_PI;
    }
    Serial.print(eul.roll_e * 180.0 / M_PI);
    Serial.print(" ");
    Serial.print(eul.pitch_e * 180.0 / M_PI);
    Serial.print(" ");
    Serial.print(eul.yaw_e * 180.0 / M_PI);
    Serial.println(" ");
    delayMicroseconds(500);
EulerAngles ToEulerAngles(Quaternion q)
{
    EulerAngles angles;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.roll_e = atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (q.w * q.y - q.z * q.x);
    if (fabs(sinp) >= 1)
        angles.pitch_e = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        angles.pitch_e = asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.yaw_e = atan2(siny_cosp, cosy_cosp);

    return angles;
}
*/

/*-----------쓰레기통-----------*/
// if (mpu.update())
// {
//     Serial.print(mpu.getYaw());
//     Serial.print(", ");
//     Serial.print(mpu.getPitch());
//     Serial.print(", ");
//     Serial.println(mpu.getRoll());
// }