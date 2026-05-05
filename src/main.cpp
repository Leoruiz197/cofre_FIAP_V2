#include <Arduino.h>
#include "hardware.h"
#include "tasks/wifi_task.h"
#include "tasks/websocket_task.h"
#include "tasks/status_task.h"
#include "tasks/mpu_task.h"

void setup() {
    Serial.begin(115200);
    randomSeed(esp_random());
    loadServoAngles();
    hardwareInit();
    setDoorClose();
    setLED2(220, 0, 70);

    Serial.println("TESTE SERVO DIRETO");

    setServo(20);
    delay(2000);

    setServo(90);
    delay(2000);

    setServo(150);
    delay(2000);

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