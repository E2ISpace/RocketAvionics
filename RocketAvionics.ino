#include "MPU9250.h"

MPU9250 mpu; // You can also use MPU9255 as is

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
    mpu.setGyroBias(2.2930535,-3.65441,1.27980);
    //2.293053522	-3.654414456	1.279805581 = 5000 data average.
    mpu.calibrateMag();
    mpu.selectFilter(QuatFilterSel::NONE);
}

void loop()
{
    mpu.update_accel_gyro_offset(); //update accel gyro
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

    // delay(100);
}