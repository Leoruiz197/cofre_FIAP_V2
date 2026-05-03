#include <Arduino.h>
#include <Wire.h>
#include "mpu_task.h"
#include "../hardware.h"
#include "../config.h"
#include "../dfplayer.h"


float threshold = 1.1; // 🔥 sensibilidade (ajuste fino)

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
    Wire.begin(21,22);

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    Serial.println("[MPU] Inicializado");
}

void mpuTask(void *pvParameters) {

    mpuInit();

    unsigned long lastTrigger = 0;

    while (true) {

        float ax, ay, az;
        readMPU(ax, ay, az);

        float impact = sqrt(ax * ax + ay * ay + az * az);

        // Serial.print("[MPU] Impacto: ");
        // Serial.println(impact);

        // 🔥 DETECÇÃO DE PANCADA
        if (impact > threshold && millis() - lastTrigger > 2000)  {

            Serial.println("[MPU] IMPACTO DETECTADO!");

            // 🔥 AÇÃO (exemplo)
            setLED1(255, 0, 0);   // vermelho
            playSound(1);        // toca som de alarme

            delay(500);

            setLED1(0, 0, 0);
        }

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}