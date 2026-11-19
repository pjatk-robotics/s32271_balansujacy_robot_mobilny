#include <Arduino.h>
#include <Bluepad32.h>
#include "mpu.h"
#include "steppers.h"
#include "pid.h"
#include "my_servo.h"
#include "pad.h"

// ===== MASZYNA STANÓW =====
enum RobotState
{
    DISARMED,
    ARMING,
    BALANCING,
    FALLEN
};
volatile RobotState currentState = DISARMED;

// ===== ZMIENNE GLOBALNE =====

volatile float joystick_drive = 0.0f;
volatile float joystick_turn = 0.0f;
float current_joystick_drive = 0.0f;
float current_joystick_turn = 0.0f;

const char *stateNames[] = {"DISARMED", "ARMING", "BALANCING", "FALLEN"};

volatile float global_pitch = 0.0f;
unsigned long last_debug_time = 0;

// ===== WĄTEK FIZYKI =====
void pidTask(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(5);

    for (;;)
    {
        float pitch = mpu_get_pitch();
        global_pitch = pitch;

        if (currentState == DISARMED)
        {
            brakeMotors();

            if (abs(pitch) > 4.0f && abs(pitch) < 30.0f)
            {
                currentState = ARMING;
            }
        }
        else if (currentState == ARMING)
        {
            pid_init();
            long dummyL, dummyR;
            getOdometry(dummyL, dummyR);
            currentState = BALANCING;
        }
        else if (currentState == BALANCING)
        {
            if (abs(pitch) > 90.0f)
            {
                brakeMotors();
                currentState = FALLEN;
            }
            else
            {
                long stepsL, stepsR;
                float speedL, speedR;

                getOdometry(stepsL, stepsR);
                pid_compute(pitch, stepsL, stepsR, joystick_drive, speedL, speedR);
                speedL += joystick_turn;
                speedR -= joystick_turn;
                float right_trim = 0.85f;
                speedR = speedR * right_trim;

                // setMotorSpeed(speedL, speedR);
            }
        }

        else if (currentState == FALLEN)
        {
            if (abs(pitch) < 2.0f)
            {
                currentState = DISARMED;
            }
        }

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void setup()
{
    Serial.begin(115200);
    delay(2000);

    setup_pad();
    setup_steppers();
    mpu_setup();
    mpu_calibrate();
    pid_init();

    xTaskCreatePinnedToCore(
        pidTask, "PID_Task", 10240, NULL, configMAX_PRIORITIES - 1, NULL, 0);

    setup_servo();
}

void loop()
{
    BP32.update();
    processControllers();

    int axisY = getLAxisY();
    int axisX = getRAxisX();

    float target_drive = (axisY / 512.0f) * 25.0f;
    float target_turn = (axisX / 512.0f) * 1500.0f;

    current_joystick_drive = (0.99f * current_joystick_drive) + (0.01f * target_drive);
    current_joystick_turn = (0.95f * current_joystick_turn) + (0.05f * target_turn);

    joystick_drive = current_joystick_drive;
    joystick_turn = current_joystick_turn;

    // --- SEKCJA ODBIORU KOMEND Z TERMINALA ---
    if (Serial.available() > 0)

    {

        char incomingChar = Serial.read();

        float val = Serial.parseFloat();

        if (incomingChar == 'p' || incomingChar == 'P')

        {

            Kp_angle_safe = val;

            Serial.printf("\n--> Zmieniono Kp_angle na: %.2f\n", Kp_angle_safe);
        }

        else if (incomingChar == 'd' || incomingChar == 'D')

        {

            Kd_angle_safe = val;

            Serial.printf("\n--> Zmieniono Kd_angle na: %.2f\n", Kd_angle_safe);
        }

        else if (incomingChar == 's' || incomingChar == 'S')

        {

            Kp_speed = val;

            Serial.printf("\n--> Zmieniono Kp_speed na: %.4f\n", Kp_speed);
        }

        else if (incomingChar == 'i' || incomingChar == 'I')

        {

            Ki_speed = val;

            Serial.printf("\n--> Zmieniono Ki_speed na: %.4f\n", Ki_speed);
        }

        else if (incomingChar == 'm' || incomingChar == 'M')

        {

            target_drive = val;

            Serial.printf("\n--> Zmieniono max_target_drive na: %.2f\n", target_drive);
        }

        // DODANO: Komenda 'L' lub 'l' do manualnego sterowania serwami nóg

        else if (incomingChar == 'l' || incomingChar == 'L')

        {

            // Przekazanie wczytanej wartości (jako int) do funkcji obsługującej serwa

            move_servo((int)val);

            Serial.printf("\n--> Ustawiono pozycję serw (nóg) na: %d\n", (int)val);
        }
    }
    // update_servo(); // Zależnie od implementacji - jeśli move_servo wysyła rozkazy bezpośrednio, update_servo() może być niepotrzebne w pętli.

    unsigned long now = millis();
    if (now - last_debug_time >= 100)
    {
        last_debug_time = now;
        RobotState currentStateCopy = currentState;

        Serial.printf("STAN: %-10s | Pitch: %6.2f | Pad Y: %4d -> Drive: %5.1f | Pad X: %4d -> Turn: %6.1f\n",
                      stateNames[currentStateCopy], global_pitch, axisY, joystick_drive, axisX, joystick_turn);
    }

    delay(5);
}