#ifndef ROCKETAVIONICS_H
#define ROCKETAVIONICS_H

#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Servo.h>
#include "MPU9250.h"
#include "LoraReboot.h"
#include <math.h>

class RocketAvionics {
public:
    RocketAvionics(int loraCsPin, int loraResetPin, int loraIrqPin, int servoPin);

    void initialize();
    void updateSensors();
    void sendLoRaData();
    void deployParachute();
    void reinitialize();

private:
    const long frequency = 9209E5;
    int loraCsPin;
    int loraResetPin;
    int loraIrqPin;
    int servoPin;
    Servo servo;
    MPU9250 mpu;
    LoraReboot receiver;
    float yaw, pitch, roll;
    float x, y, z;

    void checkParachuteDeployment();
};

#endif // ROCKETAVIONICS_H
