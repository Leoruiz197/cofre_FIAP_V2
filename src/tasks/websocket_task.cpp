#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "websocket_task.h"
#include "../websocket.h"
#include "../system_state.h"
#include "../hardware.h"
#include "../dfplayer.h"

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {

        case WStype_CONNECTED:
            Serial.println("[WS] Conectado");
            wsConnected = true;

            // 🔥 envia identificação do device
            {
                JsonDocument doc;
                doc["device"] = device_id;

                String msg;
                serializeJson(doc, msg);

                webSocket.sendTXT(msg);
            }

            break;

        case WStype_DISCONNECTED:
            Serial.println("[WS] Desconectado");
            wsConnected = false;
            break;

        case WStype_TEXT:
            Serial.println("[WS] Mensagem recebida:");
            String msg = String((char*)payload);
            Serial.println(msg);

            JsonDocument doc;
            deserializeJson(doc, msg);

            String type = doc["type"];

            if (type == "batch") {

                JsonArray commands = doc["commands"];

                for (JsonObject cmd : commands) {

                    String command = cmd["command"];



                    // ================= SERVO =================
                    if (command == "LOCK") {

                        // tenta ler como string OU número
                        String action = cmd["value"] | "";

                        Serial.println("[CMD] LOCK:");
                        Serial.println(action);

                        // ✔ modo OPEN / CLOSE
                        if (action == "OPEN") {
                            setDoorOpen();
                        }
                        else if (action == "CLOSE") {
                            setDoorClose();
                        }

                        // ✔ modo ANGLE (quando vier número)
                        else {

                            int angle = cmd["value"] | -1;

                            if (angle >= 0) {
                                Serial.print("[SERVO] Angulo direto: ");
                                Serial.println(angle);

                                setServo(angle);
                            }
                            else {
                                Serial.println("[LOCK] Valor inválido");
                            }
                        }
                    }

                    // ================= LED =================
                    if (command == "LED") {

                        String target = cmd["target"] | "";

                        int r = cmd["r"] | 0;
                        int g = cmd["g"] | 0;
                        int b = cmd["b"] | 0;

                        Serial.println("[CMD] LED:");
                        Serial.print("Target: "); Serial.println(target);
                        Serial.print("RGB: ");
                        Serial.print(r); Serial.print(", ");
                        Serial.print(g); Serial.print(", ");
                        Serial.println(b);

                        if (target == "STRIP1") {
                            setLED1(r, g, b); // 🔥 chama hardware
                        }
                        if (target == "STRIP2") {
                            setLED2(r, g, b); // 🔥 chama hardware
                        }
                    }

                    if (command == "LEDS_OFF") {
                        Serial.println("[CMD] LEDS_OFF");

                        setLED1(0, 0, 0);
                        //setLED2(0, 0, 0);
                    }

                    // ================= LED Interno =================
                    if (command == "LED_INTERNAL") {
                        bool state = false;

                        if (cmd["value"].is<bool>()) {
                            state = cmd["value"].as<bool>();
                        } else if (cmd["value"].is<int>()) {
                            state = cmd["value"].as<int>() == 1;
                        } else if (cmd["value"].is<const char*>()) {
                            String value = cmd["value"].as<const char*>();
                            state = value == "1" || value == "true" || value == "ON";
                        }

                        Serial.println("[CMD] LED_INTERNAL:");
                        Serial.println(state);

                        luzInterno(state);
                    }

                    // ================= SMOKE =================
                    if (command == "SMOKE") {
                        bool state = false;

                        if (cmd["value"].is<bool>()) {
                            state = cmd["value"].as<bool>();
                        } else if (cmd["value"].is<int>()) {
                            state = cmd["value"].as<int>() == 1;
                        } else if (cmd["value"].is<const char*>()) {
                            String value = cmd["value"].as<const char*>();
                            state = value == "1" || value == "true" || value == "ON";
                        }

                        Serial.println("[CMD] SMOKE:");
                        Serial.println(state);

                        setSmoke(state);
                    }

                    // ================= SOUND =================
                    if (command == "SOUND") {
                        String action = cmd["action"] | "";

                        Serial.println("[CMD] SOUND:");
                        Serial.println(action);

                        if (action == "PLAY") {
                            int track = cmd["track"] | 1;
                            playSound(track);
                        }

                        else if (action == "STOP") {
                            stopSound();
                        }

                        else if (action == "SET_VOLUME") {
                            int volume = cmd["value"] | 20;

                            Serial.print("[CMD] Volume recebido: ");
                            Serial.println(volume);

                            setVolume(volume);
                        }

                        else if (action == "UP") {
                            volumeUp();
                        }

                        else if (action == "DOWN") {
                            volumeDown();
                        }
                    }

                    // ================= WIN EFFECT =================
                    if (command == "WIN_EFFECT") {

                        Serial.println("[CMD] WIN_EFFECT");
                        setLED1(0, 255, 0); 
                        playSound(random(4, 8)); // som 4 a 7

                        setSmoke(true);
                        delay(2000);

                        luzInterno(true);
                        delay(500);
                        setDoorOpen();
                    }
                }
            }
            break;
    }
}

void websocketTask(void *pvParameters) {

    Serial.println("[WS] Iniciando...");

    while (true) {

        // ================= GARANTE WIFI =================
        if (!wifiReady) {
            Serial.println("[WS] Aguardando WiFi...");
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }

        // ================= TESTA DNS =================
        IPAddress ip;

        if (!WiFi.hostByName(ws_host, ip)) {
            Serial.println("[WS] DNS falhou, tentando novamente...");
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }

        Serial.println("[WS] DNS OK, conectando...");

        // ================= CONECTA =================
        webSocket.beginSSL(ws_host, atoi(ws_port), "/");
        webSocket.onEvent(webSocketEvent);
        webSocket.setReconnectInterval(5000);

        // ================= LOOP =================
        while (WiFi.status() == WL_CONNECTED) {
            webSocket.loop();
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }

        // se caiu WiFi, volta pro início
        Serial.println("[WS] WiFi caiu, reiniciando conexão...");
        wsConnected = false;

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}