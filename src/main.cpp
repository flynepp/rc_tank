#include <Arduino.h>
#include <WiFi.h>

#include "wifi_init.h"
#include "ws_server.h"
#include "OTA_setup.h"
#include "ov2640.h"
#include "http_server.h"
#include "usage.h"

camera_fb_t *fb = nullptr;
unsigned long last_camera = 0;
const unsigned long CAMERA_INTERVAL = 100;

unsigned long last_toggle = 0;
const unsigned long LIGHT_INTERVAL = 1000;

void setup()
{
    Serial.begin(115200);

    init_camera();

    wifi_init_sta();

    OTA_setup();

    ws_server_init();

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
}