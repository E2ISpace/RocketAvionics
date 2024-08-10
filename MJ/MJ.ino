#include <Servo.h>

Servo servo;
const byte servoPin = 3;
void setup()
{
    Serial.begin(9600);
    servo.attach(servoPin);
    servo.write(0); 
    
}

void loop()
{
    Serial.println("3");
    
    servo.write(72);
    delay(1000);
    servo.write(0);

    delay(3000);
}