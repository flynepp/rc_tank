#include <WiFi.h>
#include "wifi_init.h"

#ifndef WIFI_SSID
#define WIFI_SSID "WIFI_SSID"
#endif

#ifndef WIFI_PASS
#define WIFI_PASS "WIFI_PASS"
#endif

void WiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.print("Got IP: ");
        Serial.println(WiFi.localIP());
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("Disconnected. Reconnecting...");
        WiFi.reconnect();
        break;
    default:
        break;
    }
}

void wifi_init_sta()
{
    Serial.println("Connecting to Wi-Fi...");

    WiFi.onEvent(WiFiEvent);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to Wi-Fi");
}