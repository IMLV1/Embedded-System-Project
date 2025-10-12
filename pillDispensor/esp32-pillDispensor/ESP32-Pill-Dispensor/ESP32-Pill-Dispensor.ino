/*
          ⠀ ⠀⠀⠀ ⠀⠀⠀⠀⠀⢀⣀⠀⠀⠀⠀⠀⠀⢀⣀⡀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠊⠈⠑⣄⠀⠀⠀⡰⠋⠀⠙⢆⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⡰⠃⠀⠀⠀⠸⣶⣶⣶⡇⠀⠀⠀⠈⢦⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠒⠒⡶⠣⢤⠀⠀⣀⣀⠉⠉⡄⠀⢀⣶⣦⠀⠈⢷⠉⣉⡉⠀
          ⠀⠀⠀⠀⠠⠖⣾⡃⠒⠚⠀⠀⠻⠟⠀⠦⠷⠖⠀⠙⠉⠀⠀⠈⣏⠀⠉⠀
          ⠀⠀⠀⠀⢀⣾⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡆⠀⠀
          ⠀⠀⠀⢀⣞⠛⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⡀⠀
          ⠀⠀⢠⣿⣿⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢧⠀
          ⠀⢀⠿⠿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⡀
          ⠀⡜⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇
          ⢠⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇
          ⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇
          ⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇
          ⠸⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠃
          ⠀⠹⡄⠀⢠⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠏⠀
          ⠀⠀⠘⢦⣾⣿⡷⢲⣤⣄⣀⡀⠀⠀⠀⢠⠀⠀⣆⢰⠀⠘⡆⣀⠔⠁⠀⠀
          ⠀⠀⠀⠀⠉⠻⠅⣾⣿⡿⢀⣿⣿⣦⣀⣸⡀⠀⣸⠼⣆⢀⡏⠁⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠀⠀⠀⠑⠒⠁⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
*/

// ===== Include required libraries =====
#include <WiFi.h>          // WiFi functions
#include <WiFiManager.h>   // Automatic WiFi configuration
#include <PubSubClient.h>  // MQTT client library
#include <ArduinoJson.h>   // For parsing JSON
#include <Preferences.h>   // Non-volatile storage (ESP32 flash)

// ===== Standard C++ libraries =====
#include <vector>     // To store pill slots
#include <algorithm>  // For sorting

// ===== Module libraries =====
#include <Wire.h>               // I2C communication
#include <RTClib.h>             // DS3231 RTC Module
#include <ESP32Servo.h>         // Servo control
#include <LiquidCrystal_I2C.h>  // LCD 16x2 I2C


// ===== MQTT configuration =====
#define MQTT_SERVER "192.168.1.118"   // MQTT broker IP
#define MQTT_PORT 1883
#define MQTT_MAX_PACKET_SIZE 1024
#define REQUEST_ADD_TIME "esp32/add"  // Topic to receive pill times (JSON)
#define SEND_STATUS "esp32/status"    // Topic to check online/offline
#define SEND_LOG "esp32/log"


// ==== BUZZER Module ====
#define BUZZER 25

// ==== Button Module ====
#define SW 26

// ==== LED Module ====
#define LEDRED 13

// ==== DS3231 Module & Display ====
#define SQW 33
#define SCL 22
#define SDA 21

// ==== ULN2003A Stepper Motor Module ====
#define B1 19
#define B2 18
#define B3 5
#define B4 17

// ==== SERVO_MOTOR ====
#define SERVO 27

// ==== PILL_DURATION =====
#define PILL_DURATION (5 * 60 * 1000UL)  // 5 minute (ms)
#define BLINK_INTERVAL 200               // BLINK 200ms

// ===== Global objects =====
WiFiClient espClient;            // TCP client used by MQTT
PubSubClient client(espClient);  // MQTT client using espClient
Preferences PREFS;               // ESP32 flash storage
WiFiManager wm;

// ===== Module objects =====
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD 16x2 I2C
RTC_DS3231 rtc;                      // RTC Module
Servo myServo;                       // Servo motor

// ===== Task Handles =====
TaskHandle_t TaskWiFiHandle;
TaskHandle_t TaskPillHandle;

// Map Backslash for lcd
byte backslash[8] = {
  B00000,  
  B10000,  //  *
  B01000,  //   *
  B00100,  //    *
  B00010,  //     *
  B00001,  //      *
  B00000,  
  B00000
};

// ===== Struct =====
struct PillTime {
  String time;
  String list;
  int slot;
  bool triger;

  PillTime() {}
  PillTime(String t, String l, int s, bool trig) {
    time = t; 
    list = l; 
    slot = s; 
    triger = trig;
  }
};

struct PillLog {
  String date;
  String time;
  int slot;
  String list;
  bool take;

  PillLog() {}
  PillLog(String d, String tm, int s, String l, bool tk) {
    date = d;
    time = tm;
    slot = s;
    list = l;
    take = tk;
  }
};

// ===== Global variables =====
std::vector<PillTime> Slot_times;
std::vector<PillLog> pillLogs;

// ==== Buzzer && Led =====
bool handlePilling = false;
bool ledState = false;
bool buzzerState = false;

unsigned long pillStartTime = 0;
unsigned long lastBlinkTime = 0;

// ==== ButtonState =====
bool lastButtonState = HIGH;

// ==== Lcd Motion =====
static unsigned long lastDotMotion = 0;
static int countDotMotion = 0;

static unsigned long lastLCDMotion = 0;
static int countMotion = 0;

bool showStopMsg = false;
unsigned long stopMsgStart = 0;

String currentTime = "-1";

// ===== log =====
String currentList = "-1";
int currentSlot = -1;

// ===== servo =====
unsigned long servoStartTime = 0;
int targetAngle = 0;
bool servoMoving = false;

// ===== Offline/Online control =====
bool wifiAvailable = false;                
unsigned long lastWifiAttempt = 0;
unsigned long lastMQTTAttempt = 0;
const unsigned long WIFI_RETRY_INTERVAL = 30000UL; 
bool wmStarted = false;

// ===== RTC & LCD =====
void setupRTC_Module();          // Initialize and start the RTC module
void setupLCD_Moddule();         // Initialize and start the 16x2 LCD
void waitPillLcd();              // Display waiting status / pill alert
void ShowMsg(bool taken);        // Show message: "Pill Taken" or "Missed dose"

// ===== WiFi & MQTT =====
void connectWiFi();                                                  // Connect to WiFi using WiFiManager
void reconnectMQTT();                                                // Reconnect to MQTT if connection is lost
void callback(char* topic, byte* payload, unsigned int length);      // Handle incoming MQTT messages

// ===== Pill Handling =====
void checkTimes();                        // Check current time against scheduled pill times
void updatePill();                        // Update pill alert status
void stopPillAlert(bool taken);           // Stop LED/buzzer alert when pill is taken or time expired

// ===== Log =====
void createLog(int s, String l, bool tt);         // Create a pill log (slot, list, status)
void sendLogBatch();                              // Send all stored logs via MQTT

// ===== Servo =====
void handleServo(int angle);     // Rotate servo motor to the specified angle

// ===== Preferences (EEPROM) =====
void saveTimes();      // Save scheduled pill times to EEPROM
void loadTimes();      // Load scheduled pill times from EEPROM

// =======================================================================================
//                                    Task 1: WiFi + MQTT
// =======================================================================================

void TaskWiFi(void *pvParameters){
  for(;;){
    if (!WiFi.isConnected()) {
      connectWiFi();      
    }

    if (WiFi.isConnected()) {
      if (!client.connected()) {
        reconnectMQTT(); 
      }

      client.loop();

      if (client.connected() && !pillLogs.empty()) {
        sendLogBatch();
      }
    }

    // ===== Stack check =====
    // Serial.print("WiFi Task Stack Free: ");
    // Serial.println(uxTaskGetStackHighWaterMark(NULL));

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}


// =======================================================================================
//                                    Task : Pill handle
// =======================================================================================

void TaskPill(void *pvParameters){
  for(;;){
    // ===== Pill handling & LCD =====
    checkTimes();    // Check if any pill should be dispensed
    updatePill();    // Update Status Pill 
    waitPillLcd();   // Always update LCD

    // ===== Stack check =====
    // Serial.print("Pill Task Stack Free: ");
    // Serial.println(uxTaskGetStackHighWaterMark(NULL));

    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}

// =======================================================================================
//                                     Task: Pill Handling
// =======================================================================================

void setupRTC_Module() {
  if (!rtc.begin()) {
    Serial.println("RTC module is NOT found");
    delay(2000);
    // ESP.restart();
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // Set RTC to compile time
  Serial.println("setupRTC_Module");

  DateTime now = rtc.now();

  Serial.print("Current RTC time: ");
  Serial.print(now.year());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.day());
  Serial.print(" ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());
}

// =======================================================================================
//                                        LCD Display 16 x 2
// =======================================================================================

void setupLCD_Moddule() {
  lcd.init();        
  lcd.backlight();   

  lcd.setCursor(0, 0); 
  lcd.print("Pill-Dispensor"); 

  lcd.setCursor(0, 1); 
  lcd.print("Welcome <3");   

  delay(2000);
}

void waitPillLcd() {
  //Serial.println("DEBUG: Enter waitPillLcd()");

  if (showStopMsg) {
    //Serial.println("showStopMsg = true");

    if (millis() - stopMsgStart >= 3000) {
      showStopMsg = false;
      currentTime = "-1";
    }

    return;
  }

  if (handlePilling) {
    //Serial.println("handlePilling = true");

    String dots = "";

    if (millis() - lastDotMotion >= 300) {
      lastDotMotion = millis();
      countMotion = (countMotion + 1) % 5; // 0 - 4

      switch(countMotion) {
        case 0:
          dots = ".    ";
          break;
        case 1:
          dots = "..   ";
          break;
        case 2:
          dots = "...  ";
          break;
        case 3:
          dots = "..   ";
          break;
        case 4:
          dots = ".    ";
          break;
        case 5:
          dots = "     ";
          break;
      }
    }

    lcd.setCursor(0, 0);
    lcd.print("Pill ready!");
    lcd.print(dots);

    lcd.setCursor(0, 1);
    lcd.print("LOVE <3         ");

    return;
  }

  //Serial.println("Normal display zone");

  if (Slot_times.empty()) {
    lcd.setCursor(0, 0);
    lcd.print("No schedule!    ");

    lcd.setCursor(0, 1);
    lcd.print("Set time first  ");

    return;
  } 

  int PillTrigerAll = 0;
  for (size_t i = 0; i < Slot_times.size(); i++) {
    if (Slot_times[i].triger) PillTrigerAll++;
  }
  
  // Serial.print("PillTrigerAll : ");
  // Serial.println(PillTrigerAll);

  if (PillTrigerAll == Slot_times.size()) {
    lcd.setCursor(0, 0);
    lcd.print("Pills empty!    ");

    lcd.setCursor(0, 1);
    lcd.print("Refill now      ");

    return;
  }
  
  String h = "--", m = "--";
  for (size_t i = 0; i < Slot_times.size(); i++) {
    if (!Slot_times[i].triger) {
      h = Slot_times[i].time.substring(0, 2);
      m = Slot_times[i].time.substring(3, 5);
      break;
    }
  }

  if (millis() - lastLCDMotion >= 500) { 
    lastLCDMotion = millis();

    // std::string anim = "/-\\|";
    const char anim[] = {'/', '-', 0, '|'};
    countMotion = (countMotion + 1) % 4;

    lcd.setCursor(0, 0);
    lcd.print("It's time!  ");
    lcd.print(anim[countMotion]); 
    lcd.print("   ");

    lcd.setCursor(0, 1);
    lcd.print("Pill: "); 
    lcd.print(h);
    lcd.print(":");
    lcd.print(m);
    lcd.print("     ");
  }
}

void ShowMsg(bool taken) {
  showStopMsg = true;
  stopMsgStart = millis();

  if (taken) {
    lcd.setCursor(0, 0);
    lcd.print("Pill Taken!     ");

    lcd.setCursor(0, 1);
    lcd.print("Good job :)");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Missed dose!    ");

    String h = currentTime.substring(0, 2); 
    String m = currentTime.substring(3, 5); 

    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(h);
    lcd.print(":");
    lcd.print(m);
    lcd.print("     ");
  }
}

// =======================================================================================
//                          CONNECT WIFI AND SETTING MQTT SEVER
// =======================================================================================

void connectWiFi() {
  if (!wmStarted) {
    wmStarted = true;
    wm.setTimeout(5);  
    Serial.println("Starting WiFiManager portal...");

    bool res = wm.autoConnect("Pill-Dispensor"); // non-blocking
    if (res) {
      Serial.println("WiFi Connected!");
      Serial.print("IP: "); Serial.println(WiFi.localIP());
      wifiAvailable = true;

      client.setServer(MQTT_SERVER, MQTT_PORT);
      client.setCallback(callback);

      if (client.connect("ESP32Client", NULL, NULL, SEND_STATUS, 0, true, "offline")) {
        client.publish(SEND_STATUS, "online", true);
        client.subscribe(REQUEST_ADD_TIME);
        Serial.println("MQTT Connected");
      }

    } else {
      Serial.println("WiFi NOT connected, will retry...");
      wifiAvailable = false;
    }

    lastWifiAttempt = millis();
  }
}


void reconnectMQTT() {
  if (!WiFi.isConnected()) return;

  unsigned long now = millis();
  if (!client.connected() && (now - lastMQTTAttempt >= 2000)) { // reconnect ทุก 2 วิ
    lastMQTTAttempt = now;

    Serial.print("Connecting to MQTT...");

    if (client.connect("ESP32Client", NULL, NULL, SEND_STATUS, 0, true, "offline")) {
      Serial.println("connected");
      client.subscribe(REQUEST_ADD_TIME);
      client.publish(SEND_STATUS, "online", true);
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
    }
  }

  if (client.connected() && WiFi.status() != WL_CONNECTED) {
    client.publish(SEND_STATUS, "offline", true);
    client.disconnect();
    Serial.println("MQTT disconnected due to WiFi lost");
  }
}


// =======================================================================================
//                                          LOG
// =======================================================================================

void createLog(int s, String l, bool tt) {
  if (s != -1 && l != "-1") {
    DateTime now = rtc.now();

    PillLog log;
    log.date = String(now.day()) + "-" + String(now.month()) + "-" + String(now.year());

    char buffer[6]; // HH:MM + \0
    sprintf(buffer, "%02d:%02d", now.hour(), now.minute());
    log.time = buffer;
    log.slot = s;
    log.list = l;
    log.take = tt;

    pillLogs.emplace_back(log);

    currentList = "-1";
    currentSlot = -1;

    Serial.println("createLog");
    Serial.println("DATA in LOG");
    Serial.print("Date : ");
    Serial.println(log.date.c_str());
    Serial.print("Time : ");
    Serial.println(log.time.c_str());
    Serial.print("Slot : ");
    Serial.println(log.slot);
    Serial.print("List : ");
    Serial.println(log.list.c_str());
    Serial.print("Take : ");
    Serial.println(log.take ? "true" : "false");
  }
}

void sendLogBatch() {
  if (!pillLogs.empty()) {
    DynamicJsonDocument doc(8192);
    JsonArray arr = doc.createNestedArray("log");

    for (size_t i = 0; i < pillLogs.size(); i++) {
        JsonObject obj = arr.createNestedObject();
        obj["date"] = pillLogs[i].date;
        obj["time"] = pillLogs[i].time;
        obj["slot"] = pillLogs[i].slot;
        obj["list"] = pillLogs[i].list;
        obj["take"] = pillLogs[i].take;
    }

    String Buff;
    serializeJson(doc, Buff);
    client.publish(SEND_LOG, Buff.c_str(), true);

    pillLogs.clear();
    Serial.println("sendLogBatch");
    Serial.println(String("JSON ") + Buff.c_str());
  }
}

// =======================================================================================
//                                      Pill-handling
// =======================================================================================

void checkTimes() {
  DateTime now = rtc.now();

  static bool resetDone = false;
  if (!resetDone && now.hour() == 0 && now.minute() == 1) {
      for (size_t i = 0; i < Slot_times.size(); i++) {
          Slot_times[i].triger = false;
      }
      saveTimes();
      resetDone = true; 
  } 
  
  if (now.hour() != 0 || now.minute() != 1) {
      resetDone = false; 
  }

  for (size_t i = 0; i < Slot_times.size(); i++) {
    String time = Slot_times[i].time;
    int slot = Slot_times[i].slot;

    int h = time.substring(0, 2).toInt();
    int m = time.substring(3, 5).toInt();

    // Debug
    // Serial.print("now.hour() == ");
    // Serial.print(now.hour());
    // Serial.print(" && h == ");
    // Serial.print(h);
    // Serial.print(" && now.minute() == ");
    // Serial.println(now.minute());

    // if (!handlePilling) {
    //   Serial.print(now.hour());
    //   Serial.print(" == ");
    //   Serial.print(h);
    //   Serial.print(" | ");
    //   Serial.print(now.minute());
    //   Serial.print(" == ");
    //   Serial.println(m);
    // }

    if (!Slot_times[i].triger && (now.hour() > h || (now.hour() == h && now.minute() >= m))) {
      Slot_times[i].triger = true;
      handleServo(90);

      pillStartTime = millis();
      handlePilling = true;

      currentTime = Slot_times[i].time;

      saveTimes();

      currentList = Slot_times[i].list;
      currentSlot = Slot_times[i].slot;

      Serial.println("Found time");
      for (int i = 0; i < Slot_times.size(); i++) {
        Serial.print(Slot_times[i].slot);
        Serial.print("). ");
        Serial.print(Slot_times[i].list);
        Serial.print(" Time = ");
        Serial.print(Slot_times[i].time);
        Serial.print(" -> ");
        Serial.println(Slot_times[i].triger ? "triger" : "No");
      }

      break;
    }
  }
}

void stopPillAlert(bool taken) {
  handlePilling = false;
  digitalWrite(LEDRED, LOW);
  digitalWrite(BUZZER, HIGH);
  ledState = false;
  buzzerState = true;

  Serial.println("stopPillAlert");

  ShowMsg(taken);
  
}

void updatePill() {
  if (handlePilling) {
    unsigned long now = millis();

    if (now - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = now;
      ledState = !ledState;
      buzzerState = !buzzerState;

      digitalWrite(LEDRED, ledState);
      digitalWrite(BUZZER, buzzerState ? HIGH : LOW);
    }

    if (now - pillStartTime >= PILL_DURATION) {
      stopPillAlert(false);
      createLog(currentSlot, currentList, false);
      handleServo(0);

      Serial.println("Pill Failed");
    }

    bool currentState = digitalRead(SW); 
    static unsigned long lastDebounceTime = 0; 
    if(lastButtonState == HIGH && currentState == LOW && millis() - lastDebounceTime > 100){ 
      stopPillAlert(true); 
      createLog(currentSlot, currentList, true); 
      handleServo(0); 

      lastDebounceTime = millis(); 
      
      Serial.println("Button toggle"); 
    } 
    
    lastButtonState = currentState; 
  }
}

// =======================================================================================
//                                          Servo
// =======================================================================================

void handleServo(int angle) {
  myServo.write(angle);

  Serial.print("Servo : ");
  Serial.print(angle);
  Serial.println(" Degree");
  Serial.println("Servo Success");
}

// =======================================================================================
//                                        PREFERENCES
// =======================================================================================

void saveTimes() {
  PREFS.begin("pill-time", false);
  PREFS.clear();
  
  PREFS.putInt("Size", Slot_times.size());
  for (size_t i = 0; i < Slot_times.size(); i++) {
    String keyTime = "T" + String(i);
    String keyList = "L" + String(i);
    String keySlot = "S" + String(i);
    String keyTriger = "TG" + String(i);

    PREFS.putString(keyTime.c_str(), Slot_times[i].time);
    PREFS.putString(keyList.c_str(), Slot_times[i].list);
    PREFS.putInt(keySlot.c_str(), Slot_times[i].slot);
    PREFS.putString(keyTriger.c_str(), Slot_times[i].triger ? "1" : "0");
  }

  PREFS.end();

  Serial.println("saveTimes Success");
}

void loadTimes() {
  PREFS.begin("pill-time", true);
  Slot_times.clear();

  int size = PREFS.getInt("Size", 0);
  DateTime now = rtc.now();  

  for (size_t i = 0; i < size; i++) {
    String keyTime = "T" + String(i);
    String keyList = "L" + String(i);
    String keySlot = "S" + String(i);
    String keyTriger = "TG" + String(i);

    PillTime p;
    p.time = PREFS.getString(keyTime.c_str(), "");
    p.list = PREFS.getString(keyList.c_str(), "");
    p.slot = PREFS.getInt(keySlot.c_str(), 0);

    String trig = PREFS.getString(keyTriger.c_str(), "0");
    p.triger = (trig == "1");

    
    int h = p.time.substring(0,2).toInt();
    int m = p.time.substring(3,5).toInt();
    if (now.hour() > h || (now.hour() == h && now.minute() >= m)) {
      p.triger = true;
    }

    Slot_times.emplace_back(p);
  }

  PREFS.end();

  Serial.println("loadTimes Success");
  for (int i = 0; i < Slot_times.size(); i++) {
    Serial.print("Slot -> ");
    Serial.print(Slot_times[i].slot);
    Serial.print("). List : ");
    Serial.print(Slot_times[i].list.c_str());
    Serial.print(" Time : ");
    Serial.print(Slot_times[i].time.c_str());
    Serial.print(" Triger : ");
    Serial.println(Slot_times[i].triger ? "true" : "false");
  }
}


// =======================================================================================
//                                        MQTT BROKER
// =======================================================================================

void callback(char* topic, byte* payload, unsigned int length) {
  // Convert payload bytes to String
  String JSON = "";
  for (unsigned int i = 0; i < length; i++) {
    JSON += (char)payload[i];
  }

  // Debug JSON
  Serial.println("JSON Time : " + JSON);

  DynamicJsonDocument doc(8192);                            // Convert String JSON -> JsonDocument (JSON object)
  DeserializationError error = deserializeJson(doc, JSON);  // String JSON -> Subtime (Key - Value)

  if (error) {
    // When JSON Error
    Serial.println("JSON Parse Failed");
    return;
  }

  if (String(topic) == REQUEST_ADD_TIME) {
    Slot_times.clear();
    JsonArray arr = doc.as<JsonArray>();

    int count = 0;
    for (JsonObject obj : arr) {
      if (count >= 8) {
        Serial.println("Warning: Max 8 slots allowed. Extra slots ignored.");
        break;
      }

      String time = obj["time"].as<String>();
      String list = obj["list"].as<String>();
      int slot = obj["slot"].as<int>();
      Slot_times.emplace_back(time, list, slot, false);

      count++;
    }

    std::sort(Slot_times.begin(), Slot_times.end(), [](const PillTime &a, const PillTime &b) {
      int ah = a.time.substring(0, 2).toInt();
      int am = a.time.substring(3, 5).toInt();

      int bh = b.time.substring(0, 2).toInt();
      int bm = b.time.substring(3, 5).toInt();

      if (ah == bh) return am < bm;
      return ah < bh;
    });

    saveTimes();

    Serial.println("Current schedule:");
    for (int i = 0; i < Slot_times.size(); i++) {
      Serial.printf("Slot %d) %s -> %s ", Slot_times[i].slot, Slot_times[i].list.c_str(), Slot_times[i].time.c_str());
      Serial.print(" Triger : ");
      Serial.println(Slot_times[i].triger ? "true" : "false");
    }
  }
} 

// =======================================================================================
//                                          MAIN SETUP
// =======================================================================================

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA, SCL); // Initialize I2C

  // Pin Setup
  pinMode(BUZZER, OUTPUT);
  pinMode(LEDRED, OUTPUT);
  pinMode(SW, INPUT_PULLUP);

  // Step Moter
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(B3, OUTPUT);
  pinMode(B4, OUTPUT);

  digitalWrite(BUZZER, HIGH);
  digitalWrite(LEDRED, LOW);

  // Servo Setup
  myServo.attach(SERVO);
  myServo.write(0);

  setupRTC_Module();    // Initialize RTC
  setupLCD_Moddule();   // Initialize LCD
  lcd.createChar(0, backslash); // Create Backslash


  loadTimes();    // Load stored pill times (do before connect for LCD update)

  // MQTT setup
  client.setBufferSize(MQTT_MAX_PACKET_SIZE);
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);

  // try to conncect Wifi Frist 
  connectWiFi();   // Connect to WiFi

  // ===== Create Tasks =====
  xTaskCreatePinnedToCore(TaskWiFi, "WiFiTask", 4096, NULL, 1, &TaskWiFiHandle, 0);
  xTaskCreatePinnedToCore(TaskPill, "PillTask", 8192, NULL, 1, &TaskPillHandle, 1);
}

// =======================================================================================
//                                          MAIN LOOP
// =======================================================================================

void loop() {
  
}