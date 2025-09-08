#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

void ina219_setup()
{
    if (!ina219.begin())
    {
        Serial.println("Failed to find INA219 chip");
    }
    else
    {
        Serial.println("Find INA219 chip");
    }
}

float get_voltage()
{
    float shuntvoltage = ina219.getShuntVoltage_mV();
    float busvoltage = ina219.getBusVoltage_V();

    float loadvoltage = busvoltage + (shuntvoltage / 1000);

    return loadvoltage;
}