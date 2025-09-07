#include "ws_server.h"
#include <ArduinoJson.h>
#include <Arduino.h>

#include <utility>
#include <cmath>
#include <vector>
using namespace std;

#define SG90_PIN 4
#define FS90R1_PIN 2
#define FS90R2_PIN 14

const int freq = 50;
const int resolution = 16;

int deg = 90;
float rotateDeg = 30.0;

pair<float, float> transfer(float x, float y)
{
    // 差速映射
    float u = y + x;
    float v = y - x;

    // 限制输出在单位圆内
    float len = sqrt(u * u + v * v);
    if (len > 1.0f)
    {
        u /= len;
        v /= len;
    }

    return make_pair(u, v);
}
vector<float> control()
{
    if (lastMsg[0] == '\0')
    {
        vector<float> result = {0.0, 0.0, 0.0};
        return result;
    }

    // ["0.00, 0.00", "-0.72"]
    JsonDocument controlMsg;

    deserializeJson(controlMsg, lastMsg);

    const char *vecStr = controlMsg[0];
    const char *turretStr = controlMsg[1];

    float x, y;
    sscanf(vecStr, "%f, %f", &x, &y);
    float z = atof(turretStr);

    auto p = transfer(x, y);

    vector<float> result = {p.first, p.second, z};

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

    ledcWrite(channel, pulseWidthToDuty(PWM));
}

void sg90(float speed)
{
    int channel = 0;

    int destination = deg + int(speed * rotateDeg);

    destination = destination < 0 ? 0 : destination;
    destination = destination > 180 ? 180 : destination;

    // 500 ~ 1500 ~ 2500
    // 0 ~ 90 ~ 180
    int PWM = 500 + 2000 / 180 * destination;

    ledcWrite(channel, pulseWidthToDuty(PWM));
}
