#include <Adafruit_MPU6050.h>

int attempts = 0;
Adafruit_MPU6050 mpu;

void mpu_6050_setup()
{
    while (!mpu.begin() && attempts < 3)
    {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    if (attempts < 3)
    {
        Serial.println("Found a MPU-6050 sensor");
        mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
        mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
    }
    else
    {
        Serial.println("Failed to find a MPU-6050 sensor");
    }
}
