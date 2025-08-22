#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define WIFI_SSID "Ukikaze"
#define WIFI_PASS "woaini7758258"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
               void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.printf("Client connected: %u\n", client->id());
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.printf("Client disconnected: %u\n", client->id());
    }
    else if (type == WS_EVT_DATA)
    {
        Serial.printf("Received: %.*s\n", len, data);
        // Echo back
        client->text((char *)data);
    }
}

void setup()
{
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi connected");

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);
    server.begin();
}

void loop()
{
    ws.cleanupClients(); // 必须调用以清理断开的客户端
}
