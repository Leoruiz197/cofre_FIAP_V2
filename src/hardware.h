#pragma once

void hardwareInit();
void setServo(int angle);
void setLED1(int r, int g, int b);
void setLED2(int r, int g, int b);
void playSound(int track);
void stopSound();
void setSmoke(bool state);