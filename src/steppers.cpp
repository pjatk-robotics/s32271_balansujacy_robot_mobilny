#include "steppers.h"
#include <math.h>

#define DIR1_PIN 32  
#define STEP1_PIN 25
#define DIR2_PIN 21  
#define STEP2_PIN 18

#define MOTOR1_INVERT false 
#define MOTOR2_INVERT false  

hw_timer_t *stepTimer = NULL;
portMUX_TYPE stepMux = portMUX_INITIALIZER_UNLOCKED;

volatile uint32_t motor1_step_interval_us = 0;
volatile uint32_t motor2_step_interval_us = 0;
volatile uint32_t motor1_last_step_us = 0;
volatile uint32_t motor2_last_step_us = 0;

volatile long motor1_absolute_steps = 0;
volatile long motor2_absolute_steps = 0;
volatile int8_t motor1_dir_sign = 1;
volatile int8_t motor2_dir_sign = 1;

void IRAM_ATTR onStepTimer()
{
    portENTER_CRITICAL_ISR(&stepMux);
    uint32_t now = micros();

    if (motor1_step_interval_us > 0 && (uint32_t)(now - motor1_last_step_us) >= motor1_step_interval_us)
    {
        digitalWrite(STEP1_PIN, HIGH);
        motor1_last_step_us = now;
        motor1_absolute_steps += motor1_dir_sign; 
        for (int i = 0; i < 30; i++) { __asm__ __volatile__("nop"); }
        digitalWrite(STEP1_PIN, LOW);
    }

    if (motor2_step_interval_us > 0 && (uint32_t)(now - motor2_last_step_us) >= motor2_step_interval_us)
    {
        digitalWrite(STEP2_PIN, HIGH);
        motor2_last_step_us = now;
        motor2_absolute_steps += motor2_dir_sign; 
        for (int i = 0; i < 30; i++) { __asm__ __volatile__("nop"); }
        digitalWrite(STEP2_PIN, LOW);
    }
    portEXIT_CRITICAL_ISR(&stepMux);
}

void setup_steppers()
{
    pinMode(DIR1_PIN, OUTPUT);
    pinMode(STEP1_PIN, OUTPUT);
    pinMode(DIR2_PIN, OUTPUT);
    pinMode(STEP2_PIN, OUTPUT);

    digitalWrite(STEP1_PIN, LOW);
    digitalWrite(STEP2_PIN, LOW);

    stepTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(stepTimer, &onStepTimer, true);
    timerAlarmWrite(stepTimer, 25, true); 
    timerAlarmEnable(stepTimer);
}

void setMotorSpeed(float speedL, float speedR)
{
    speedL = constrain(speedL, -MAX_STEPS_PER_SEC, MAX_STEPS_PER_SEC);
    speedR = constrain(speedR, -MAX_STEPS_PER_SEC, MAX_STEPS_PER_SEC);

    bool dirL = speedL >= 0;
    bool dirR = speedR >= 0; 

    if (MOTOR1_INVERT) dirL = !dirL;
    if (MOTOR2_INVERT) dirR = !dirR;

    digitalWrite(DIR1_PIN, dirL ? HIGH : LOW);
    digitalWrite(DIR2_PIN, dirR ? HIGH : LOW);
    uint32_t intervalL = (fabs(speedL) > 0.1f) ? (uint32_t)(1000000.0f / fabs(speedL)) : 0;
    uint32_t intervalR = (fabs(speedR) > 0.1f) ? (uint32_t)(1000000.0f / fabs(speedR)) : 0;

    portENTER_CRITICAL(&stepMux);
    motor1_dir_sign = speedL >= 0 ? 1 : -1;
    motor2_dir_sign = speedR >= 0 ? 1 : -1;
    
    motor1_step_interval_us = intervalL;
    motor2_step_interval_us = intervalR;
    portEXIT_CRITICAL(&stepMux);
}

void brakeMotors()
{
    portENTER_CRITICAL(&stepMux);
    motor1_step_interval_us = 0;
    motor2_step_interval_us = 0;
    portEXIT_CRITICAL(&stepMux);
}

void getOdometry(long &stepsL, long &stepsR)
{
    portENTER_CRITICAL(&stepMux);
    stepsL = motor1_absolute_steps;
    stepsR = motor2_absolute_steps;
    
    motor1_absolute_steps = 0;
    motor2_absolute_steps = 0;
    portEXIT_CRITICAL(&stepMux);
}