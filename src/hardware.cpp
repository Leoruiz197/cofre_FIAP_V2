#include "config.h"
#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>
#include "system_state.h"
#include "hardware.h"
#include "dfplayer.h"

Servo servo;

Adafruit_NeoPixel led1(LED_COUNT, LED1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led2(LED_COUNT, LED2_PIN, NEO_GRB + NEO_KHZ800);

int strip1R = 0;
int strip1G = 0;
int strip1B = 0;

int strip2R = 0;
int strip2G = 0;
int strip2B = 0;

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
    servoAngle = angle;
    servo.write(angle);
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

    for(int i = 0; i < LED_COUNT; i++) {
        led2.setPixelColor(i, led2.Color(r, g, b));
    }
    led2.show();

    Serial.println("[HW] LED2 atualizado");
}
