#ifndef STEPPERS_H
#define STEPPERS_H

#include <Arduino.h>

const int MAX_STEPS_PER_SEC = 35000.0; 

void setup_steppers();

void setMotorSpeed(float speedL, float speedR);

void brakeMotors();

void getOdometry(long &stepsL, long &stepsR);

#endif