#include <Arduino.h>
#include <ESP32Servo.h>
#include "my_servo.h"

// Zabezpieczenie przed wpisaniem zbyt małego i dużego kąta
#define MAX_ANGLE_CHANGE 180
#define MAX_JOYSTICK 512

Servo servo1;
Servo servo2;

// Inicjalizacja pozycji (Start od 180 stopni z jednej, 0 z drugiej)
int targetAngle1 = 180;
int targetAngle2 = 0;
int currentAngle1 = 180;
int currentAngle2 = 0;

// Piny dla ESP32
const int SERVO1_PIN = 33;
const int SERVO2_PIN = 19;

void setup_servo()
{
    // Konfiguracja sprzętowych timerów ESP32 dla sygnału PWM (50 Hz)
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    
    // Ustawienie standardowych impulsów dla serw (500us - 2400us)
    servo1.setPeriodHertz(50);
    servo2.setPeriodHertz(50);

    // Podpięcie pod zadeklarowane piny (D33 i D19)
    servo1.attach(SERVO1_PIN, 500, 2400);
    servo2.attach(SERVO2_PIN, 500, 2400);

    // Ustawienie pozycji początkowej
    servo1.write(currentAngle1);
    servo2.write(currentAngle2);
}

void move_servo(int amount)
{
    // Zabezpieczenie przed wpisaniem skrajnych głupot z klawiatury
    if (amount < 0) amount = 0;
    if (amount > MAX_ANGLE_CHANGE) amount = MAX_ANGLE_CHANGE;

    // Serwo 1: Startuje z 180 i maleje (np. 180 - 30 = 150)
    targetAngle1 = 180 - (amount+5);
    
    // Serwo 2 (Lustrzane): Startuje z 0 i rośnie (np. 0 + 30 = 30)
    targetAngle2 = amount;

    // Wykonanie ruchu na serwomechanizmach
    servo1.write(targetAngle1);
    servo2.write(targetAngle2);
    
    // Zapis aktualnych wartości
    currentAngle1 = targetAngle1;
    currentAngle2 = targetAngle2;
}