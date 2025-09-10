#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

bool ina219_setup()
{
    if (!ina219.begin())
    {
        Serial.println("Failed to find INA219 chip");
        return false;
    }
    else
    {
        Serial.println("Find INA219 chip");
        return true;
    }
}

float get_voltage()
{
    float loadvoltage = 12.0;

    if (ina219.begin())
    {
        float shuntvoltage = 0;
        float busvoltage = 0;

        shuntvoltage = ina219.getShuntVoltage_mV();
        busvoltage = ina219.getBusVoltage_V();

        if (!isnan(shuntvoltage) && !isnan(busvoltage))
        {
            loadvoltage = busvoltage + (shuntvoltage / 1000.0f);
        }
    }

    return loadvoltage;
}