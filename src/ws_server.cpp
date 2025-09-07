#include "ws_server.h"
#include <WiFi.h>

#define USE_SERIAL Serial // ESP32-CAM 直接用 Serial

// WebSocket 实例
WebSocketsServer *g_ws = nullptr;
WebSocketsServer webSocket = WebSocketsServer(81);
int Connected = 0;
char lastMsg[256];

static void hexdump(const void *mem, uint32_t len, uint8_t cols = 16)
{
    const uint8_t *src = (const uint8_t *)mem;
    USE_SERIAL.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
    for (uint32_t i = 0; i < len; i++)
    {
        if (i % cols == 0)
            USE_SERIAL.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
        USE_SERIAL.printf("%02X ", *src);
        src++;
    }
    USE_SERIAL.printf("\n");
}

// WebSocket 回调
static void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        USE_SERIAL.printf("[%u] Disconnected!\n", num);
        Connected--;
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket.remoteIP(num);
        USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n",
                          num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocket.sendTXT(num, "Connected");
        Connected++;
    }
    break;
    case WStype_TEXT:
        strncpy(lastMsg, (char *)payload, length);
        lastMsg[length] = '\0';
        break;
    case WStype_BIN:
        USE_SERIAL.printf("[%u] get binary length: %u\n", num, length);
        hexdump(payload, length);
        break;
    default:
        break;
    }
}

// 外部调用初始化函数
void ws_server_init()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        g_ws = &webSocket;
        webSocket.begin();
        webSocket.onEvent(webSocketEvent);
        Serial.println("WebSocket server started on port 81.");
    }
    else
    {
        Serial.println("Wi-Fi not connected yet, cannot start WebSocket server.");
    }
}

// 外部循环函数
void ws_server_loop()
{
    webSocket.loop();
}

void ws_server_send(const String &msg)
{
    webSocket.broadcastTXT(msg.c_str());
}

void ws_server_send_image(uint8_t *buf, size_t len)
{
    webSocket.broadcastBIN(buf, len);
}