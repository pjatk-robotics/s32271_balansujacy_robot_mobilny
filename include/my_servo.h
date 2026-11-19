#include <Wire.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#ifndef MY_SERVO_H
#define SERMY_SERVO_HVO_H

void setup_servo();
void move_servo(int amount1);
void update_servo();

#endif