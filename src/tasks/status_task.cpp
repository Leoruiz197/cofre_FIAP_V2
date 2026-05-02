#include <Arduino.h>
#include <ArduinoJson.h>

#include "status_task.h"
#include "websocket_task.h"
#include "system_state.h"


// ================= TASK =================
void statusTask(void *pvParameters) {

    Serial.println("[STATUS] Iniciando...");

    while (true) {

        JsonDocument doc;

        doc["type"] = "status";
        doc["device"] = "cofre1";
        doc["state"] = "online";

        doc["servo"] = servoAngle;
        doc["smoke"] = smokeActive;

        JsonObject sound = doc["sound"].to<JsonObject>();
        sound["playing"] = soundPlaying;
        sound["track"] = currentTrack;

        String msg;
        serializeJson(doc, msg);

        wsSend(msg);

        Serial.println("[STATUS] enviado");

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}