#include "pid.h"

float Kp_angle_safe = 1500.0f; 
float Kd_angle_safe = 10.0f; 

float Kp_speed = 0.12f; 
float Ki_speed = 0.0f; 

float angle_error_previous = 0.0f;
float speed_error_integral = 0.0f;


float filtered_derivative = 0.0f;
float filtered_speed = 0.0f;

const float TARGET_PITCH = -35.29f; 

void pid_init()
{
    angle_error_previous = 0.0f;
    speed_error_integral = 0.0f;
    filtered_derivative = 0.0f;
    filtered_speed = 0.0f;
}

void pid_compute(float current_pitch, long stepsL, long stepsR, float joystick_cmd, float &outSpeedL, float &outSpeedR)
{
    // predkosc
    float instant_speed = (stepsL + stepsR) / 2.0f;
    
    filtered_speed = (0.85f * filtered_speed) + (0.15f * instant_speed);

    float speed_error = joystick_cmd - filtered_speed; 
    
    speed_error_integral += speed_error;

    speed_error_integral = constrain(speed_error_integral, -500.0f, 500.0f);

    float speed_pid_output = (Kp_speed * speed_error) + (Ki_speed * speed_error_integral);
    speed_pid_output = constrain(speed_pid_output, -10.0f, 10.0f);

    //kat
    float target_pitch = TARGET_PITCH - speed_pid_output; 
    float angle_error = current_pitch - target_pitch;
    
    float raw_derivative = (angle_error - angle_error_previous) / 0.005f;
    angle_error_previous = angle_error;


    filtered_derivative = (0.50f * filtered_derivative) + (0.50f * raw_derivative);

  
    float base_output = (Kp_angle_safe * angle_error) + (Kd_angle_safe * filtered_derivative);

    outSpeedL = -base_output;
    outSpeedR = -base_output;
}