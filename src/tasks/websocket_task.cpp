#include <Arduino.h>
#include <ArduinoJson.h>
#include "websocket_task.h"
#include "../websocket.h"   // 🔥 IMPORTANTE

const char* WS_HOST = "websocket-cofre.onrender.com";
const int WS_PORT = 443;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {

        case WStype_CONNECTED:
            Serial.println("[WS] Conectado");
            break;

        case WStype_DISCONNECTED:
            Serial.println("[WS] Desconectado");
            break;

        case WStype_TEXT:
            Serial.println("[WS] Mensagem recebida");
            break;
    }
}

void websocketTask(void *pvParameters) {

    webSocket.beginSSL(WS_HOST, WS_PORT, "/");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);

    while (true) {
        webSocket.loop();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}