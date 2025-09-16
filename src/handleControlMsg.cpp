#include "ws_server.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include "esp_system.h"

#include <utility>
#include <cmath>
#include <vector>
using namespace std;

#define SG90_PIN 16
#define FS90R1_PIN 12
#define FS90R2_PIN 13

const int freq = 50;
const int resolution = 12;

int deg = 90;
float rotateDeg = 90.0;

bool rebootFlag = false;

pair<float, float> transfer(float x, float y)
{
    // Step 1: 水平翻转
    float x_flip = -x;
    float y_flip = y;

    // Step 2: 顺时针旋转 45°
    float cos45 = sqrt(2.0f) / 2.0f;
    float sin45 = sqrt(2.0f) / 2.0f;

    float u = x_flip * cos45 + y_flip * sin45;
    float v = -x_flip * sin45 + y_flip * cos45;

    // Serial.printf("x: %.2f, y: %.2f => u: %.2f, v: %.2f\n", x, y, u, v);

    return make_pair(u, v);
}

vector<float> control()
{
    vector<float> result = {0.0, 0.0, 0.0};

    if (lastMsg[0] == '\0')
    {
        return result;
    }

    // ["0.00, 0.00", "-0.72"]
    // ["reboot"]
    JsonDocument controlMsg;

    deserializeJson(controlMsg, lastMsg);

    if (controlMsg[0].as<String>() == "reboot")
    {
        rebootFlag = true;

        ws_server_send("\"confirm\"");

        return result;
    }

    const char *vecStr = controlMsg[0];
    const char *turretStr = controlMsg[1];

    float x, y;
    sscanf(vecStr, "%f, %f", &x, &y);
    float z = atof(turretStr);

    auto p = transfer(x, y);

    result = {p.first, p.second, z};

    return result;
}

void initMotor()
{
    ledcSetup(0, freq, resolution);
    ledcSetup(1, freq, resolution);
    ledcSetup(2, freq, resolution);

    ledcAttachPin(SG90_PIN, 0);
    ledcAttachPin(FS90R1_PIN, 1);
    ledcAttachPin(FS90R2_PIN, 2);
}

int pulseWidthToDuty(int pulse_us)
{
    // duty = pulse / period * 2^resolution
    // period = 1/freq = 20ms = 20000us
    return (pulse_us * (1 << resolution)) / 20000;
}

void fs90r(string which, float speed)
{
    float direction = 1.0;
    int channel = 1;

    if (which == "left")
    {
        direction = 1.0;
        channel = 1;
    }
    else
    {
        direction = -1.0;
        channel = 2;
    }

    int PWM = int(1500.0 + direction * speed * 300.0);

    char buf[50];
    sprintf(buf, "LedcWrite: channel %d, PWM %d, Duty %d. ", channel, PWM, pulseWidthToDuty(PWM));
    Serial.print(buf);
    Serial.print(which.c_str());
    Serial.println();

    ledcWrite(channel, pulseWidthToDuty(PWM));
}

void sg90(float rad)
{
    int channel = 0;

    int destination = deg + int(rad * rotateDeg);

    destination = destination < 0 ? 0 : destination;
    destination = destination > 180 ? 180 : destination;

    // 500 ~ 1500 ~ 2500
    // 0 ~ 90 ~ 180
    int PWM = 500 + 2000 / 180 * destination;

    // char buf[50];
    // sprintf(buf, "LedcWrite: channel %d, PWM %d, Duty %d.", channel, PWM, pulseWidthToDuty(PWM));
    // Serial.print(buf);
    // Serial.println();

    ledcWrite(channel, pulseWidthToDuty(PWM));
}
