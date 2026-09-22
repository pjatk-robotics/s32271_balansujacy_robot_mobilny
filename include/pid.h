#ifndef PID_H
#define PID_H

#include <Arduino.h>

extern float Kp_angle_safe;
extern float Kd_angle_safe;

extern float Kp_speed;
extern float Ki_speed;

void pid_init();

void pid_compute(float current_pitch, long stepsL, long stepsR, float joystick_cmd, float &outSpeedL, float &outSpeedR);

#endif