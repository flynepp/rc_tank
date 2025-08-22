#ifndef WS_SERVER_H
#define WS_SERVER_H

#include <WebSocketsServer.h>

// 初始化 WebSocket
void ws_server_init();

// WebSocket 循环调用（在 loop 或 app_main 里执行）
void ws_server_loop();

#endif // WS_SERVER_H