#include <Arduino.h>
#include "wifi_init.h"

void setup()
{
    Serial.begin(115200);

    // 调用 Wi-Fi 初始化
    wifi_init_sta();

    Serial.println("Setup finished.");
}

void loop()
{
    // 这里可以添加其他任务逻辑
}
