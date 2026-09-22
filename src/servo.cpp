#include "my_servo.h"

#define MAX_ANGLE 70
#define MAX_JOYSTICK 512

Servo servo1;
Servo servo2;
int targetAngle1 = 50;
int targetAngle2 = 180 - 50;
int currentAngle1 = 50;
int currentAngle2 = 180 - 50;

void setup_servo()
{
    // servo1.attach(5);
    // servo2.attach(25);
    // servo1.write(50);
    //  servo2.write(130);

    
}

void move_servo(int amount)
{
        servo1.write(amount);
     servo2.write(180-amount);
}

void update_servo()
{
    int krok = 3;

    if (currentAngle1 < targetAngle1)
        currentAngle1 = min(currentAngle1 + krok, targetAngle1);
    else if (currentAngle1 > targetAngle1)
        currentAngle1 = max(currentAngle1 - krok, targetAngle1);

    if (currentAngle2 < targetAngle2)
        currentAngle2 = min(currentAngle2 + krok, targetAngle2);
    else if (currentAngle2 > targetAngle2)
        currentAngle2 = max(currentAngle2 - krok, targetAngle2);

    servo1.write(currentAngle1);
    servo2.write(currentAngle2);
}