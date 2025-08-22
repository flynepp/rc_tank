#ifndef WS_SERVER_H
#define WS_SERVER_H

#include <ESPAsyncWebServer.h>

// 声明 WebSocket 对象和初始化函数
extern AsyncWebSocket ws;
void ws_init(AsyncWebServer &server);

#endif // WS_SERVER_H
