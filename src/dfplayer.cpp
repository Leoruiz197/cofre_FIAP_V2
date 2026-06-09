#include "config.h"
#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>
#include "system_state.h"

HardwareSerial dfSerial(2);
DFRobotDFPlayerMini dfPlayer;

bool df_ok = false;

// ================= INIT =================
void initDFPlayer() {

    dfSerial.begin(9600, SERIAL_8N1, DF_RX, DF_TX);

    if (dfPlayer.begin(dfSerial)) {

        df_ok = true;

        dfPlayer.volume(25);

        Serial.println("[DF] DFPlayer OK");

    } else {

        df_ok = false;

        Serial.println("[DF] Erro DFPlayer!");
    }
}

// ================= PLAY =================
void playSound(int track) {

    if (!df_ok) {
        Serial.println("[DF] Não inicializado");
        return;
    }

    currentTrack = track;
    soundPlaying = true;

    dfPlayer.play(track);

    Serial.println("[DF] Tocando faixa");
}

// ================= STOP =================
void stopSound() {

    if (!df_ok) return;

    dfPlayer.stop();

    soundPlaying = false;

    Serial.println("[DF] Parado");
}

// ================= VOLUME =================
void setVolume(int volume) {

    if (!df_ok) return;

    if (volume < 0) volume = 0;
    if (volume > 50) volume = 50;

    dfPlayer.volume(volume);

    Serial.print("[DF] Volume setado: ");
    Serial.println(volume);
}

// ================= AUMENTAR =================
void volumeUp() {

    if (!df_ok) return;

    dfPlayer.volumeUp();

    Serial.println("[DF] Volume aumentado");
}

// ================= DIMINUIR =================
void volumeDown() {

    if (!df_ok) return;

    dfPlayer.volumeDown();

    Serial.println("[DF] Volume diminuído");
}