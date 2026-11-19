#ifndef PID_H
#define PID_H

#include <Arduino.h>

extern float Kp_angle_safe;
extern float Kd_angle_safe;

extern float Kp_speed;
extern float Ki_speed;
// Zmienne pomocnicze do logowania
extern float debug_angle_error;
extern float debug_pitch_rate;
extern float debug_base_output;
void pid_init();

void pid_compute(float current_pitch, long stepsL, long stepsR, float joystick_cmd, float &outSpeedL, float &outSpeedR);

#endif