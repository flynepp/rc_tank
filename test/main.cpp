#include <iostream>
#include <cstring>

using namespace std;

const int freq = 50;
const int resolution = 16;
int deg = 90;
float rotateDeg = 30.0;

void ledcWrite(int channel, int duty)
{
    cout << "[ledcWrite] channel=" << channel << ", duty=" << duty << endl;
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

int main()
{
    sg90(0.5);
    fs90r("left", 0.8);
    fs90r("right", 0.6);

    return 0;
}
