#ifndef MPU_H
#define MPU_H

#include <Arduino.h>
#include <Wire.h>

void mpu_setup();
void mpu_calibrate();
float mpu_get_pitch();

#endif