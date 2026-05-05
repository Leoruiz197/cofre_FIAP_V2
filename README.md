# 🔐 Cofre Inteligente FIAP V2

Projeto desenvolvido para a FIAP com o objetivo de criar um **cofre interativo e conectado**, utilizando ESP32, sensores e atuadores, com controle remoto via WebSocket.

---

## 🚀 Visão Geral

O sistema simula um cofre inteligente com:

- 🔐 Trava controlada por servo motor
- 💡 Iluminação com LEDs endereçáveis (NeoPixel)
- 🔊 Reprodução de áudio com DFPlayer Mini
- 🌫️ Máquina de fumaça
- 📡 Comunicação em tempo real via WebSocket
- 📶 Configuração dinâmica de rede (WiFiManager)
- 📊 Detecção de impacto com MPU6050

---

## 🧠 Funcionalidades

- Controle remoto via backend
- Execução de comandos em lote (batch)
- Configuração persistente (memória interna)
- Feedback visual e sonoro
- Detecção de impacto com resposta automática

---

## 🧩 Arquitetura do Projeto
src/
├── main.cpp
├── config.h
├── hardware.cpp
├── hardware.h
├── dfplayer.cpp
├── dfplayer.h
├── system_state.cpp
├── system_state.h
├── websocket.cpp
├── websocket.h
│
├── tasks/
│   ├── mpu_task.cpp
│   ├── mpu_task.h
│   ├── status_task.cpp
│   ├── status_task.h
│   ├── websocket_task.cpp
│   ├── websocket_task.h
│   ├── wifi_task.cpp
│   ├── wifi_task.h

---

## ⚙️ Hardware Utilizado

- ESP32
- MPU6050 (acelerômetro)
- DFPlayer Mini
- Servo Motor (trava)
- LEDs NeoPixel (2 fitas)
- Módulo de fumaça
- Fonte externa adequada

---

## 📡 Comunicação (WebSocket)

O dispositivo se conecta a um servidor WebSocket e recebe comandos no formato JSON.

### Exemplo de comando:

```json
{
  "type": "batch",
  "commands": [
    {
      "command": "LOCK",
      "action": "OPEN"
    },
    {
      "command": "LED",
      "target": "ALL",
      "effect": "SOLID",
      "r": 255,
      "g": 0,
      "b": 0
    }
  ]
}

🛠️ Como rodar o projeto (PlatformIO + VS Code)
📌 1. Instalar o VS Code

Baixe e instale:

👉 https://code.visualstudio.com/

📌 2. Instalar o PlatformIO
Abra o VS Code
Vá em Extensions (Ctrl + Shift + X)
Busque por:
PlatformIO IDE
Clique em Install

⏳ Aguarde a instalação completa (pode demorar alguns minutos)

📌 3. Clonar o repositório

No terminal do VS Code:

git clone https://github.com/Leoruiz197/cofre_FIAP_V2.git

Ou baixe o ZIP e extraia.

📌 4. Abrir o projeto

No VS Code:

File → Open Folder

Selecione a pasta do projeto.

📌 5. Conectar o ESP32
Conecte o ESP32 via USB
Verifique a porta COM

📌 6. Compilar o projeto

Clique no botão:

✔️ Build

ou use:

pio run

📌 7. Enviar o firmware

Clique em:

➡️ Upload

ou:

pio run --target upload

📌 8. Monitor serial

Para ver logs:

pio device monitor

ou botão:

🔌 Monitor


# 📶 Configuração Wi-Fi

Na primeira execução (ou após reset):

O ESP cria uma rede:
Cofre-Setup
Conecte pelo celular ou PC
Acesse:
192.168.4.1
Configure:
Wi-Fi
Servidor WebSocket
Porta
Device ID

🔄 Reset de configuração

Para resetar o Wi-Fi:

👉 Segure o botão BOOT ao ligar o ESP32

🎵 Áudio (DFPlayer)

Os arquivos devem estar no cartão SD:

0001.mp3
0002.mp3
0003.mp3
0004.mp3

Formato recomendado:

MP3
128kbps

⚠️ Observações importantes
O sistema utiliza tarefas (FreeRTOS)
Evitar uso de delay() em partes críticas
Alimentação deve ser adequada (principalmente servo e LEDs)

