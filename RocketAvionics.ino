#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Servo.h>
#include "MPU9250.h"
#include <math.h>
#include "LoRareboot.h"

extern MPU9250 mpu; // MPU9250 센서 객체 생성
extern Servo servo;
class RocketAvionics {
public:
    RocketAvionics(int loraCsPin, int loraResetPin, int loraIrqPin, int servoPin)
        : loraCsPin(loraCsPin), loraResetPin(loraResetPin), loraIrqPin(loraIrqPin), servoPin(servoPin) {}

    void initialize() {
        // LoRa 초기화
        Serial.begin(9600);
        while (!Serial);
        Serial.println("LoRa Sender");

        if (!LoRa.begin(frequency)) {
            Serial.println("Starting LoRa failed!");
            while (1);
        }

        // MPU9250 초기화
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
        
        if (!mpu.setup(0x68, setting)) {
            Serial.println("MPU9250 setup failed");
            while (1);
        }

        mpu.verbose(true);
        mpu.setAccBias(-946.44, 633.1475, -1104.61);
        mpu.setGyroBias(280.12, -488.685, 167.025);
        mpu.calibrateMag();
        mpu.selectFilter(QuatFilterSel::MAHONY);

        // 서보 모터 초기화
        servo.attach(servoPin);
        servo.write(0);
    }

    void updateSensors() {
        if (mpu.update()) {
            yaw = mpu.getYaw();
            pitch = mpu.getPitch();
            roll = mpu.getRoll();

            // 방향 벡터 계산 (yaw와 pitch 사용)
            float yawRad = yaw * DEG_TO_RAD;
            float pitchRad = pitch * DEG_TO_RAD;
            x = cos(pitchRad) * cos(yawRad);
            y = cos(pitchRad) * sin(yawRad);
            z = sin(pitchRad);

            sendLoRaData();
            checkParachuteDeployment();
        }
    }

    void sendLoRaData() {
        LoRa.beginPacket();
        LoRa.print(yaw);
        LoRa.print("/ ");
        LoRa.print(pitch);
        LoRa.print("/ ");
        LoRa.print(roll);
        LoRa.print("/ ");
        LoRa.endPacket();
    }

    void deployParachute() {
        Serial.println("Parachute Deployed");
        servo.write(72); // 서보 모터를 72도 위치로 이동
        delay(1000); // 1초 대기
        servo.write(0); // 서보 모터를 원래 위치로 복귀
    }

    void reinitialize() {
        initialize();
    }

private:
    const long frequency = 9209E5;
    int loraCsPin;
    int loraResetPin;
    int loraIrqPin;
    int servoPin;
    float yaw, pitch, roll;
    float x, y, z;

    void checkParachuteDeployment() {
        float targetX = 0;
        float targetY = 0;
        float targetZ = 1;
        float dotProduct = x * targetX + y * targetY + z * targetZ;
        float vectorMagnitude = sqrt(x * x + y * y + z * z) * sqrt(targetX * targetX + targetY * targetY + targetZ * targetZ);
        float angle = acos(dotProduct / vectorMagnitude) * RAD_TO_DEG;

        if (angle <= 0 || angle >= 180) {
            deployParachute();
        }
    }
};

RocketAvionics rocket(10, 9, 2, 22);
LoraReboot receiver(10,9,2);
bool state = 0;
// pinMode(csPin, OUTPUT);
// digitalWrite(csPin, HIGH);

void setup() {
    rocket.initialize();
    receiver.begin(9209E5);
    delay(1000);

}

void loop() { 
    rocket.updateSensors();
}