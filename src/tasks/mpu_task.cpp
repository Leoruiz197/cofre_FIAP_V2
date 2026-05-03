#include <Arduino.h>
#include <Wire.h>
#include "mpu_task.h"
#include "../hardware.h"
#include "../config.h"
#include "../dfplayer.h"

float threshold = 1.05;

// Guarda a cor anterior do LED 1
extern int strip1R;
extern int strip1G;
extern int strip1B;

void readMPU(float &ax, float &ay, float &az) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);

    Wire.requestFrom((uint8_t)MPU_ADDR, (uint8_t)6, (bool)true);

    if (Wire.available() >= 6) {
        int16_t rawX = Wire.read() << 8 | Wire.read();
        int16_t rawY = Wire.read() << 8 | Wire.read();
        int16_t rawZ = Wire.read() << 8 | Wire.read();

        ax = rawX / 16384.0;
        ay = rawY / 16384.0;
        az = rawZ / 16384.0;
    } else {
        Serial.println("[MPU] Erro leitura I2C");
    }
}

void mpuInit() {
    Wire.begin(21, 22);

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    Serial.println("[MPU] Inicializado");
}

void impactFeedback() {
    int oldR = strip1R;
    int oldG = strip1G;
    int oldB = strip1B;

    setLED1(255, 0, 0);
    playSound(1);

    delay(500);

    setLED1(oldR, oldG, oldB);
}

void mpuTask(void *pvParameters) {
    mpuInit();

    unsigned long lastTrigger = 0;

    while (true) {
        float ax = 0;
        float ay = 0;
        float az = 0;

        readMPU(ax, ay, az);

        float impact = sqrt(ax * ax + ay * ay + az * az);

        if (impact > threshold && millis() - lastTrigger > 2000) {
            lastTrigger = millis();

            Serial.println("[MPU] IMPACTO DETECTADO!");

            impactFeedback();
        }

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}