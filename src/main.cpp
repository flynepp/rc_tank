#include <Arduino.h>
#include <WiFi.h>
#include "wifi_init.h"
#include "ws_server.h"

void setup()
{
    Serial.begin(115200);

    // 调用 Wi-Fi 初始化
    wifi_init_sta();

    ws_server_init();

    Serial.println("Setup finished.");
}

void loop()
{
    // 这里可以添加其他任务逻辑
    ws_server_loop();
}
