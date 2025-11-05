# 🩺💊 ESP32 IoT Smart Healthcare System
(Heart Rate Monitor + Automatic Pill Dispenser + EMQX + MongoDB + Node-RED + Vue.js + Docker)

## 🧠 Overview
An IoT-based Smart Healthcare System using ESP32, designed to monitor heart rate and automatically dispense pills based on patient schedules.  
All data flows through MQTT (EMQX), processed by Node-RED, stored in MongoDB, and visualized with Vue.js — all containerized with Docker.

---

## 🔄 Typical Data Flow
```
[Heart Rate ESP32] ──► topic/heartrate ─┐
                                         ├──► Node-RED ─► MongoDB ─► Vue.js Dashboard
[Pill Dispenser ESP32] ◄── esp32/add ────┘
          │
          ├──► esp32/status
          └──► esp32/log
```

---

## 🧩 Components

| Component | Description |
|------------|--------------|
| 🩺 ESP32 Heart Rate Monitor | Measures BPM via MAX30102, displays on OLED, and sends MQTT messages. |
| 💊 ESP32 Pill Dispenser | Reads schedules and dispenses pills automatically using stepper motor. |
| ☁️ EMQX MQTT Broker | Handles all MQTT communication between devices and Node-RED. |
| 🔄 Node-RED | Processes device data, manages schedules, and connects to MongoDB and frontend. |
| 🗄️ MongoDB | Stores users, schedules, heart rate logs, and dispensing history. |
| 🌐 Vue.js Frontend | Displays real-time heart rate, pill schedule, and system status. |
| 🐳 Docker | Deploys all services in a containerized environment. |

---

## 🔁 System Workflow

### 🩺 1. Heart Rate Monitoring
- The ESP32 Heart Rate Monitor uses the MAX30102 sensor to detect the user’s pulse.  
- It calculates BPM (beats per minute) and displays the value on an OLED screen.  
- The BPM data is then published to the EMQX MQTT Broker under the topic `/esp32/heartrate`.

### 💊 2. Pill Dispensing
- The ESP32 Pill Dispenser subscribes to the MQTT schedule topics and retrieves time data from RTC.  
- When the current time matches a scheduled slot, it triggers a stepper motor to rotate and dispense the pill.  
- The action is logged and sent back to MQTT topic `/esp32/dispense/status`.

### ☁️ 3. MQTT Message Routing
EMQX Broker manages communication between devices and server.  
Heart rate and pill data are routed to Node-RED for further processing.

| Topic | Description |
|--------|-------------|
| /esp32/heartrate | BPM data from Heart Rate device |
| /esp32/dispense/status | Pill dispense confirmation |
| /esp32/schedule/update | Schedule updates from server |

### 🔄 4. Node-RED Processing

#### 📡 MQTT Communication Workflow
The system communicates via MQTT topics handled by the EMQX Broker.  
Each ESP32 device publishes and subscribes to specific topics for real-time interaction.

| Topic | Direction | Description |
|--------|------------|-------------|
| esp32/add | ⬇️ Subscribed by Pill Dispenser | Receives pill schedule data in JSON format from Node-RED or the web dashboard. Example: `{"time":"08:00","slot":1}` |
| esp32/status | ⬆️ Published by Pill Dispenser | Reports device online/offline status to the server. Example: `{"device":"pill_dispenser","status":"online"}` |
| esp32/log | ⬆️ Published by Pill Dispenser | Sends log messages or dispense results to Node-RED for database storage. Example: `{"slot":2,"time":"08:00","action":"dispensed"}` |
| topic/heartrate | ⬆️ Published by Heart Rate Monitor | Sends real-time BPM readings from the MAX30102 sensor to the MQTT broker. Example: `{"bpm":78,"time":"10:32:15"}` |

### 🗄️ 5. MongoDB Data Storage

| Collection | Content |
|-------------|----------|
| users | User profiles and authentication |
| heartrate | Time-stamped BPM readings |
| schedule | Medication schedules and slots |
| dispense_logs | Pill dispensing history |

### 🌐 6. Frontend Visualization (Vue.js)
The Vue.js web dashboard shows:
- Real-time heart rate graph (live via WebSocket)
- Next pill schedule and status
- Dispense history and alerts
- Users can manage schedules or view patient data directly from the browser.

### 🐳 7. Docker Integration
All backend components (EMQX, MongoDB, Node-RED, and Vue.js) run as containers, simplifying setup and ensuring consistent deployment.

---
