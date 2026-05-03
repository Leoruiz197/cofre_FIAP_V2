#include <Arduino.h>
#include <ArduinoJson.h>
#include "status_task.h"
#include "../websocket.h"
#include "../system_state.h"

String lastStatusJson = "INIT";
int statusSemAlteracaoCount = 0;

void statusTask(void *pvParameters) {
    while (true) {

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

        if (msg != lastStatusJson) {
            wsSend(msg);

            Serial.println("[STATUS] enviado:");
            Serial.println(msg);

            lastStatusJson = msg;
            statusSemAlteracaoCount = 0;
        } else {
            statusSemAlteracaoCount++;

            Serial.print("[STATUS] sem alteração: ");
            Serial.println(statusSemAlteracaoCount);

            if (statusSemAlteracaoCount >= 3) {
                wsSend(msg);

                Serial.println("[STATUS] heartbeat online enviado:");
                Serial.println(msg);

                statusSemAlteracaoCount = 0;
            }
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}