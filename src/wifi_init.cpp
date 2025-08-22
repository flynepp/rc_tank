#include <WiFi.h>
#include "wifi_init.h"

#define WIFI_SSID "Ukikaze"
#define WIFI_PASS "woaini7758258"

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
}