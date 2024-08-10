#ifndef PARACHUTE_H
#define PARACHUTE_H

#include <Servo.h>
#include "MPU9250.h"

class Parachute {
public:
    Parachute();
    void begin();
    void update();

private:
    MPU9250 mpu;
    Servo servo;

    void deployParachute();
};

#endif
