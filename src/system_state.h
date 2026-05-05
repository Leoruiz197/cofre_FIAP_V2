#pragma once
extern int strip1R;
extern int strip1G;
extern int strip1B;

extern int strip2R;
extern int strip2G;
extern int strip2B;

extern bool wifiReady;
extern bool wsConnected;

extern int servoAngle;
extern bool smokeActive;

extern bool soundPlaying;
extern int currentTrack;

extern bool shouldSaveConfig;

extern char ws_host[40];
extern char ws_port[6];
extern char device_id[20];

extern int doorOpenAngle;
extern int doorCloseAngle;
extern bool doorOpen;
extern bool internalLightOn;