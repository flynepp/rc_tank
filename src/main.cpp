#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include <Wire.h>

#include "wifi_init.h"
#include "ws_server.h"
#include "OTA_setup.h"
#include "ov2640.h"
#include "usage.h"
#include "handleControlMsg.h"
#include "INA219.h"

using namespace std;

camera_fb_t *fb = nullptr;
unsigned long last_camera = 0;
const unsigned long CAMERA_INTERVAL = 100;

unsigned long last_control = 0;
const unsigned long CONTROL_INTERVAL = 20;

void setup()
{
    Serial.begin(115200);

    Wire.begin(16, 0);

    init_camera();

    wifi_init_sta();

    OTA_setup();

    ws_server_init();

    ina219_setup();

    Serial.println("Setup finished.");
}

void loop()
{
    ws_server_loop();
    ArduinoOTA.handle();

    unsigned long now = millis();

    if (now - last_camera >= CAMERA_INTERVAL && Connected)
    {
        unsigned long taskStart = millis();

        camera_fb_t *fb = take_picture();
        if (fb)
        {
            ws_server_send_image(fb->buf, fb->len);
            return_picture(fb);
        }

        unsigned long taskEnd = millis();
        unsigned long taskElapsed = taskEnd - taskStart;

        const char *usage = getUsage(taskElapsed, CAMERA_INTERVAL);
        ws_server_send(usage);

        last_camera = now;
    }

    if (now - last_control >= CONTROL_INTERVAL && Connected)
    {
        vector<float> controlData;

        controlData = control();

        Serial.printf("x: %.2f, y: %.2f, z: %.2f\n", controlData[0], controlData[1], controlData[2]);

        last_control = now;
    }
}