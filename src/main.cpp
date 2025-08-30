#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>

#include "wifi_init.h"
#include "ws_server.h"
#include "OTA_setup.h"
#include "mpu_6050.h"
#include "display.h"

#include <vector>
#include <numeric>

std::vector<float> gravitys;

const float G = 9.80665f;

void setup()
{
    Serial.begin(115200);

    wifi_init_sta();

    OTA_setup();

    ws_server_init();

    Wire.begin(15, 14);

    mpu_6050_setup();

    display_setup();

    Serial.println("Setup finished.");
}

void i2cScanner()
{
    byte error, address;
    int nDevices = 0;

    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.print(address, HEX);
            Serial.println(" !");
            nDevices++;
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");

    delay(2000);
}

void loop()
{
    ws_server_loop();

    ArduinoOTA.handle();

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // i2cScanner();

    float norm = sqrtf(a.acceleration.x * a.acceleration.x +
                       a.acceleration.y * a.acceleration.y +
                       a.acceleration.z * a.acceleration.z);

    gravitys.push_back(norm);

    if (gravitys.size() > 10)
    {
        gravitys.erase(gravitys.begin());
    }

    float avg = std::accumulate(gravitys.begin(), gravitys.end(), 0.0f) / gravitys.size();

    float factor = G / avg;

    display.clearDisplay();
    display.setCursor(0, 0);

    display.println("Accelerometer - m/s^2");
    display.print(a.acceleration.x * factor, 2);
    display.print(", ");
    display.print(a.acceleration.y * factor, 2);
    display.print(", ");
    display.print(a.acceleration.z * factor, 2);
    display.println("");

    display.println("Gyroscope - rps");
    display.print(g.gyro.x, 1);
    display.print(", ");
    display.print(g.gyro.y, 1);
    display.print(", ");
    display.print(g.gyro.z, 1);
    display.println("");

    display.display();
    delay(100);
}
