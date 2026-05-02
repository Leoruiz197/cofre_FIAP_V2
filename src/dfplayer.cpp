#include "config.h"
#include <DFRobotDFPlayerMini.h>
#include "system_state.h"

HardwareSerial dfSerial(2);
DFRobotDFPlayerMini player;

bool df_ok = false;

void dfInit() {
    dfSerial.begin(9600, SERIAL_8N1, DF_RX, DF_TX);

    if (player.begin(dfSerial)) {
        df_ok = true;
        player.volume(25);
    }
}

void playTrack(int track) {
    if (!df_ok) return;

    currentTrack = track;
    player.play(track);
    soundPlaying = true;
}

void stopTrack() {
    if (!df_ok) return;

    player.stop();
    soundPlaying = false;
}