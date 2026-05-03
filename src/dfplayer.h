#pragma once

#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

extern HardwareSerial dfSerial;
extern DFRobotDFPlayerMini dfPlayer;

void playSound(int track);
void stopSound();
void initDFPlayer();

void setVolume(int volume);
void volumeUp();
void volumeDown();