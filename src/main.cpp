#include <Arduino.h>
#include "hardware.h"
#include "tasks/wifi_task.h"
#include "tasks/websocket_task.h"
#include "tasks/status_task.h"
#include "tasks/mpu_task.h"

void setup() {
    Serial.begin(115200);
    randomSeed(esp_random());
    hardwareInit();

    // WIFI
    xTaskCreatePinnedToCore(
        wifiTask,
        "WiFi Task",
        10000,
        NULL,
        1,
        NULL,
        0
    );

    // WEBSOCKET
    xTaskCreatePinnedToCore(
        websocketTask,
        "WebSocket Task",
        10000,
        NULL,
        1,
        NULL,
        1
    );

    // STATUS
    xTaskCreatePinnedToCore(
        statusTask,
        "Status Task",
        15000,
        NULL,
        1,
        NULL,
        1
    );
    // MPU
    xTaskCreatePinnedToCore(
        mpuTask,
        "MPU Task",
        4096,
        NULL,
        1,
        NULL,
        1
    );
}

void loop() {
    // vazio
}