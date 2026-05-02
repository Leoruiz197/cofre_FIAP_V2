#include <Arduino.h>
#include <WiFiManager.h>
#include "wifi_task.h"

void wifiTask(void *pvParameters) {

    Serial.println("[WiFi] Iniciando...");

    WiFiManager wm;

    if (!wm.autoConnect("Cofre-Setup")) {
        Serial.println("[WiFi] Falha, reiniciando...");
        ESP.restart();
    }

    Serial.println("[WiFi] Conectado!");
    Serial.println(WiFi.localIP());

    vTaskDelete(NULL); // finaliza a task
}