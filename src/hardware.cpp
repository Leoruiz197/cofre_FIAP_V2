#include "config.h"
#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>
#include "system_state.h"
#include "hardware.h"
#include "dfplayer.h"
#include <Preferences.h>

Preferences hardwarepreferences;

Servo servo;

Adafruit_NeoPixel led1(LED_COUNT, LED1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led2(LED_COUNT, LED2_PIN, NEO_GRB + NEO_KHZ800);

void loadServoAngles() {
    hardwarepreferences.begin("cofre", true);

    doorOpenAngle = hardwarepreferences.getInt("openAngle", 30);
    doorCloseAngle = hardwarepreferences.getInt("closeAngle", 160);

    hardwarepreferences.end();

    Serial.println("[EEPROM] Angulos carregados:");
    Serial.print("Open: ");
    Serial.println(doorOpenAngle);
    Serial.print("Close: ");
    Serial.println(doorCloseAngle);
}

void saveServoAngles(int openAngle, int closeAngle) {
    doorOpenAngle = constrain(openAngle, 0, 180);
    doorCloseAngle = constrain(closeAngle, 0, 180);

    hardwarepreferences.begin("cofre", false);

    hardwarepreferences.putInt("openAngle", doorOpenAngle);
    hardwarepreferences.putInt("closeAngle", doorCloseAngle);

    hardwarepreferences.end();

    Serial.println("[EEPROM] Angulos salvos:");
    Serial.print("Open: ");
    Serial.println(doorOpenAngle);
    Serial.print("Close: ");
    Serial.println(doorCloseAngle);
}

void hardwareInit() {
    pinMode(SMOKE_PIN, OUTPUT);
    digitalWrite(SMOKE_PIN, HIGH);

    Serial.println("[HW] Inicializando servo...");
    servo.setPeriodHertz(50);
    servo.attach(SERVO_PIN, 500, 2500);

    led1.begin();
    led1.clear();
    led1.show();
    
    led2.begin();
    led2.clear();
    led2.show();

    initDFPlayer();
}

void setServo(int angle) {
    angle = constrain(angle, 0, 180);

    servo.write(angle);
    servoAngle = angle;

    Serial.print("[SERVO] Angulo direto: ");
    Serial.println(angle);
}

void setDoorOpen() {
    servo.write(doorOpenAngle);
    doorOpen = true;

    servoAngle = doorOpenAngle;

    Serial.print("[SERVO] Abrindo porta: ");
    Serial.println(doorOpenAngle);
}

void setDoorClose() {
    servo.write(doorCloseAngle);
    doorOpen = false;

    servoAngle = doorCloseAngle;

    Serial.print("[SERVO] Fechando porta: ");
    Serial.println(doorCloseAngle);
}

void setSmoke(bool state) {
    smokeActive = state;
    digitalWrite(SMOKE_PIN, state ? LOW : HIGH);
}

void setLED1(int r, int g, int b) {
    strip1R = r;
    strip1G = g;
    strip1B = b;

    for(int i = 0; i < LED_COUNT; i++) {
        led1.setPixelColor(i, led1.Color(r, g, b));
    }
    led1.show();

    Serial.println("[HW] LED1 atualizado");
}

void setLED2(int r, int g, int b) {
    strip2R = r;
    strip2G = g;
    strip2B = b;

    for(int i = 0; i < LED2_COUNT; i++) {
        led2.setPixelColor(i, led2.Color(r, g, b));
    }
    led2.show();

    Serial.println("[HW] LED2 atualizado");
}

void luzInterno(bool on) {
    internalLightOn = on;

    for (int i = LED2_COUNT; i < LED2_COUNT + LED_INT_COUNT; i++) {
        led2.setPixelColor(
            i,
            on ? led2.Color(255, 255, 255) : led2.Color(0, 0, 0)
        );
    }

    led2.show();

    Serial.println("[HW] LED Interno atualizado");
}

