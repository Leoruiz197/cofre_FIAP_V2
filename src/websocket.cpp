#include "websocket.h"
#include "./system_state.h"

WebSocketsClient webSocket;

void wsSend(String msg) {
    if (!wsConnected) return;
    webSocket.sendTXT(msg);
}

void wsInit(const char* host, int port) {
    webSocket.beginSSL(host, port, "/");
    //webSocket.begin(host, port, "/");
    webSocket.setReconnectInterval(5000);
}

void wsLoop() {
    webSocket.loop();
}
