#include <ArduinoOTA.h>

void OTA_setup()
{
    Serial.println("Setting up OTA...");

    ArduinoOTA.setHostname("esp32cam-ota");

    ArduinoOTA.setPassword("123456");

    ArduinoOTA.onStart([]()
                       { Serial.println("Start OTA"); });
    ArduinoOTA.onEnd([]()
                     { Serial.println("End OTA"); });

    ArduinoOTA.begin();

    Serial.println("OTA Ready");
}