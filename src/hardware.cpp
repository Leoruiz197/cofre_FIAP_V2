#include "config.h"
#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>
#include "system_state.h"

Servo servo;

Adafruit_NeoPixel led1(LED_COUNT, LED1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led2(LED_COUNT, LED2_PIN, NEO_GRB + NEO_KHZ800);


void hardwareInit() {
    pinMode(SMOKE_PIN, OUTPUT);
    digitalWrite(SMOKE_PIN, HIGH);

    servo.attach(SERVO_PIN);

    led1.begin();
    led2.begin();
}

void setServo(int angle) {
    servoAngle = angle;
    servo.write(angle);
}

void setSmoke(bool state) {
    smokeActive = state;
    digitalWrite(SMOKE_PIN, state ? LOW : HIGH);
}

void setLED(Adafruit_NeoPixel &strip, int r, int g, int b) {
    for (int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, strip.Color(r, g, b));
    }
    strip.show();
}