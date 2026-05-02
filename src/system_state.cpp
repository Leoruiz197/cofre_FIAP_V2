#include "system_state.h"

bool wifiReady = false;
bool wsConnected = false;

int servoAngle = 0;
bool smokeActive = false;

bool soundPlaying = false;
int currentTrack = 0;

bool shouldSaveConfig = false;

char ws_host[40] = "websocket-cofre.onrender.com";
char ws_port[6] = "443";
char device_id[20] = "cofre";