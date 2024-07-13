#include "MPU9250.h"
#include <math.h>
MPU9250 mpu; // You can also use MPU9255 as is
Avionics test;
bool state = 0;
void setup()
{

    //LoRa
    Serial.begin(9600); 
    while (!Serial);

    Serial.println("LoRa Sender");

    if (!LoRa.begin(9209E5)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

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

    // delay(5000);
    mpu.verbose(true);
    // calibrate anytime you want to
    // mpu.calibrateAccelGyro();
    mpu.setAccBias(-946.44, 633.1475, -1104.61);
    mpu.setGyroBias(280.12, -488.685, 167.025);
    mpu.calibrateMag();
    mpu.selectFilter(QuatFilterSel::MAHONY);
}

void loop()
{
    
    /**
     * initial Heading and .. others
     * 
     */
    if (!state)
    {
        test.initialize();
        state = 1;
    }
    if (mpu.update())
    {
        Serial.print(mpu.getYaw());
        Serial.print(", ");
        Serial.print(mpu.getPitch());
        Serial.print(", ");
        Serial.println(mpu.getRoll());

        LoRa.beginPacket();
        
        LoRa.print(mpu.getYaw());
        LoRa.print("/ ");

        LoRa.print(mpu.getPitch());
        LoRa.print("/ ");

        LoRa.print(mpu.getRoll());
        LoRa.print("/ ");

        LoRa.endPacket();
    }

    /**
     * @brief reInitialize remote!
     * @param none
     * @if button on
     */
    if (digitalRead(13) == HIGH)
    {
        test.reinitialize();
        state = 1;
    }

    if (TRUE/*heading where*/)
    {
        test.deployParachute();
    }
}

class Avionics
{
private:
public:
    virtual void initialize() = 0;
    virtual void deployParachute() = 0;
};