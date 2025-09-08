#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

void ina219_setup()
{
    if (!ina219.begin())
    {
        Serial.println("Failed to find INA219 chip");
        while (1)
        {
            delay(10);
        }
    }
}

float get_voltage()
{

    float busvoltage = ina219.getBusVoltage_V();

    return busvoltage;
}