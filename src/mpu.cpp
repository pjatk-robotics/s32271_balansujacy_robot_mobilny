#include "mpu.h"
#include <math.h>

const int MPU_ADDR = 0x68;
float current_pitch = 0.0f;
float gyro_x_offset = 0.0f;

const float ALPHA = 0.996f;
const float DT = 0.005f; 
bool angle_initialized = false;
float pitch_offset = 0.0f;

void mpu_setup()
{
    // I2C (SDA=22, SCL=23) 
    Wire.begin(22, 23);
    Wire.setClock(400000);

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission(true);

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x1A);
    Wire.write(0x03);
    Wire.endTransmission(true);

    delay(100); 
}

void mpu_calibrate()
{
    Serial.println("Rozpoczecie kalibracji MPU6050... NIE RUSZAJ ROBOTA!");
    long gyro_sum = 0;
    float accel_pitch_sum = 0;
    const int samples = 500;

    for (int i = 0; i < samples; i++)
    {
        Wire.beginTransmission(MPU_ADDR);
        Wire.write(0x3B); 
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_ADDR, 14, true);
        
        int16_t ax = Wire.read() << 8 | Wire.read();
        int16_t ay = Wire.read() << 8 | Wire.read();
        int16_t az = Wire.read() << 8 | Wire.read();
        Wire.read(); Wire.read();
        int16_t gx = Wire.read() << 8 | Wire.read(); 

        gyro_sum += gx;
        
      
        float a_pitch = atan2((float)ay, sqrt((float)ax * ax + (float)az * az)) * 180.0f / PI;
        accel_pitch_sum += a_pitch;
        
        delay(2);
    }

    gyro_x_offset = (float)gyro_sum / samples / 131.0f;
    
    pitch_offset = accel_pitch_sum / samples;

    Serial.printf("Kalibracja zakonczona. Offset Gyro X: %.2f | Offset Pitch: %.2f\n", gyro_x_offset, pitch_offset);
}

float mpu_get_pitch()
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 14, true);

    int16_t ax = Wire.read() << 8 | Wire.read();
    int16_t ay = Wire.read() << 8 | Wire.read();
    int16_t az = Wire.read() << 8 | Wire.read();
    Wire.read(); Wire.read(); 
    int16_t gx = Wire.read() << 8 | Wire.read();
    Wire.read(); Wire.read(); 
    Wire.read(); Wire.read(); 

    
    float accel_pitch = atan2((float)ay, sqrt((float)ax * ax + (float)az * az)) * 180.0f / PI;
    float gyro_rate = (gx / 131.0f) - gyro_x_offset;

    
    if (!angle_initialized)
    {
        current_pitch = accel_pitch;
        angle_initialized = true;
    }

   
    current_pitch = ALPHA * (current_pitch + gyro_rate * DT) + (1.0f - ALPHA) * accel_pitch;

    return current_pitch - pitch_offset; 
}