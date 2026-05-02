#include <Arduino.h>
#include <ArduinoJson.h>
#include "status_task.h"
#include "../websocket.h"
#include "../system_state.h"

void statusTask(void *pvParameters) {

    while (true) {

        // 🔥 NÃO ENVIA SE NÃO ESTIVER CONECTADO
        if (!wsConnected) {
            Serial.println("[STATUS] WS não conectado, aguardando...");
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }

        JsonDocument doc;

        doc["type"] = "status";
        doc["device"] = device_id;
        doc["state"] = "online";

        doc["servo"] = servoAngle;
        doc["smoke"] = smokeActive;

        JsonObject sound = doc["sound"].to<JsonObject>();
        sound["playing"] = soundPlaying;
        sound["track"] = currentTrack;

        String msg;
        serializeJson(doc, msg);

        wsSend(msg);

        Serial.println("[STATUS] enviado:");
        Serial.println(msg);

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}