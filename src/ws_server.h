#ifndef WS_SERVER_H
#define WS_SERVER_H

#include <WebSocketsServer.h>

extern WebSocketsServer *g_ws;

// 初始化 WebSocket
void ws_server_init();

// WebSocket 循环调用（在 loop 或 app_main 里执行）
void ws_server_loop();

void ws_server_send(const String &msg);

#endif // WS_SERVER_H