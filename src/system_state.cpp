#include "system_state.h"

bool wifiReady = false;
bool wsConnected = false;

int servoAngle = 0;
bool doorOpen = false;
bool internalLightOn = false;
bool smokeActive = false;

bool soundPlaying = false;
int currentTrack = 0;

bool shouldSaveConfig = false;

char ws_host[40] = "websocket-cofre.onrender.com";
char ws_port[6] = "443";
char device_id[20] = "cofre";

int doorOpenAngle = 30;   // padrão aberto
int doorCloseAngle = 160; // padrão fechado

int strip1R = 0;
int strip1G = 0;
int strip1B = 0;

int strip2R = 0;
int strip2G = 0;
int strip2B = 0;