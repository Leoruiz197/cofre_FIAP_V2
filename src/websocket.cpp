#include "websocket.h"

WebSocketsClient webSocket;

void wsSend(String msg) {
    webSocket.sendTXT(msg);
}

void wsInit(const char* host, int port) {
    webSocket.beginSSL(host, port, "/");
    webSocket.setReconnectInterval(5000);
}

void wsLoop() {
    webSocket.loop();
}
