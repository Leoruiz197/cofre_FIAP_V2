#pragma once

extern int strip1R;
extern int strip1G;
extern int strip1B;

extern int strip2R;
extern int strip2G;
extern int strip2B;



void hardwareInit();
void setServo(int angle);
void setLED1(int r, int g, int b);
void setLED2(int r, int g, int b);
void playSound(int track);
void stopSound();
void setSmoke(bool state);
void luzInterno(bool on);

void loadServoAngles();
void saveServoAngles(int openAngle, int closeAngle);
void setDoorOpen();
void setDoorClose();