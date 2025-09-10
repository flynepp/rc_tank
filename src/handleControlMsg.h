#ifndef HANDLECONTROLMSG_H
#define HANDLECONTROLMSG_H

#include <string>
#include <vector>
using std::string;
using std::vector;

extern bool rebootFlag;

vector<float> control();
void initMotor();
void fs90r(string which, float speed);
void sg90(float speed);

#endif // HANDLECONTROLMSG_H