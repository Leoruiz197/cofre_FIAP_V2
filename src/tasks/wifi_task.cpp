#include <Arduino.h>
#include <WiFiManager.h>
#include "wifi_task.h"
#include "../system_state.h"
#include <Preferences.h>
#include "./dfplayer.h"

Preferences preferences;

#define RESET_BUTTON_PIN 0

// const char* DEFAULT_SSID = "cf1";
// const char* DEFAULT_PASS = "12982026";
// const char* DEFAULT_DEVICE_ID = "cofre1";


void wifiTask(void *pvParameters) {

    Serial.println("[WiFi] Iniciando...");

    pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
    strcpy(device_id, DEFAULT_DEVICE_ID);

    // ================= CARREGA CONFIG SALVA =================
    preferences.begin("config", true);

    String savedHost = preferences.getString("host", "");
    String savedPort = preferences.getString("port", "");
    String savedDevice = preferences.getString("device", "");

    preferences.end();

    if (savedHost.length() > 0) strcpy(ws_host, savedHost.c_str());
    if (savedPort.length() > 0) strcpy(ws_port, savedPort.c_str());
    if (savedDevice.length() > 0) strcpy(device_id, savedDevice.c_str());

    Serial.println("[WiFi] Tentando WiFi padrão...");

    WiFi.begin(DEFAULT_SSID, DEFAULT_PASS);

    unsigned long startAttempt = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000) {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[WiFi] Conectado via WiFi padrão!");
    } else {
        Serial.println("\n[WiFi] Falhou, abrindo portal...");
    }

    // ================= WIFI MANAGER =================
    WiFiManager wm;

    WiFiManagerParameter custom_host("host", "Host Backend", ws_host, 40);
    WiFiManagerParameter custom_port("port", "Porta", ws_port, 6);
    WiFiManagerParameter custom_device("device", "Device ID", device_id, 20);

    wm.addParameter(&custom_host);
    wm.addParameter(&custom_port);
    wm.addParameter(&custom_device);

    // ================= RESET MANUAL =================
    unsigned long pressStart = 0;

    if (digitalRead(RESET_BUTTON_PIN) == LOW) {

        pressStart = millis();

        while (digitalRead(RESET_BUTTON_PIN) == LOW) {

            if (millis() - pressStart > 3000) {

                Serial.println("[WiFi] Reset + Portal forçado");

                // limpa WiFi
                wm.resetSettings();

                // 🔥 limpa Preferences
                preferences.begin("config", false);
                preferences.clear();
                preferences.end();

                delay(1000);

                wm.startConfigPortal("Cofre-Setup");
                break;
            }

            delay(10);
        }
    }

    // ================= AUTO CONNECT =================
    playSound(3);
    if (!wm.autoConnect("Cofre-Setup")) {
        Serial.println("[WiFi] Falha, reiniciando...");
        ESP.restart();
    }

    Serial.println("[WiFi] Conectado!");

    // ================= ATUALIZA CONFIG =================
    if (strlen(custom_host.getValue()) > 0)
        strcpy(ws_host, custom_host.getValue());

    if (strlen(custom_port.getValue()) > 0)
        strcpy(ws_port, custom_port.getValue());

    if (strlen(custom_device.getValue()) > 0)
        strcpy(device_id, custom_device.getValue());

    // ================= SALVA CONFIG =================
    preferences.begin("config", false);

    preferences.putString("host", ws_host);
    preferences.putString("port", ws_port);
    preferences.putString("device", device_id);

    preferences.end();

    Serial.println("[WiFi] Configuração:");
    Serial.println(ws_host);
    Serial.println(ws_port);
    Serial.println(device_id);

    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());

    Serial.print("DNS: ");
    Serial.println(WiFi.dnsIP());

    // ================= ESPERA ESTABILIZAÇÃO =================
    Serial.println("[WiFi] Aguardando estabilização da rede...");
    delay(3000);

    // ================= TESTE DNS =================
    IPAddress resolvedIP;
    int tentativas = 0;

    while (!WiFi.hostByName(ws_host, resolvedIP)) {

        Serial.println("[WiFi] DNS falhou, tentando novamente...");
        delay(2000);

        tentativas++;

        if (tentativas > 5) {
            Serial.println("[WiFi] DNS falhando muito, reiniciando...");
            ESP.restart();
        }
    }

    Serial.println("[WiFi] DNS OK:");
    Serial.println(resolvedIP);

    delay(1000);

    Serial.println("[WiFi] Finalizado com sucesso");

    // 🔥 libera outras tasks
    wifiReady = true;

    vTaskDelete(NULL);
}