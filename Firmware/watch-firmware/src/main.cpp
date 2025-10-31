#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>

// ========================
// Heartrate Sensor Variables
// ========================
MAX30105 hr_sensor;

const byte BUFF_SIZE = 4;
byte rates[BUFF_SIZE];
byte rate_idx = 0;
unsigned long lastBeat = 0;

float beat_per_min;
int beatAvg = 0;

// ========================
// Display Variables
// ========================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET 16
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ========================
// Timing and Display Toggle
// ========================
unsigned long last_time = 0;
unsigned int period = 500;
bool togle = true;

// ========================
// WiFi & MQTT Configuration
// ========================
WiFiManager wm;

const char *mqtt_server = "20.244.31.58";
WiFiClient espClient;
PubSubClient client(espClient);
const char *watch_topic = "topic/heartrate";

TaskHandle_t mqttTaskHandle;

// ========================
// Heart icon bitmap (for OLED)
// ========================
const unsigned char heart[] PROGMEM = {
    0b00001100, 0b00110000,
    0b00011110, 0b01111000,
    0b00111111, 0b11111100,
    0b01111111, 0b11111110,
    0b01111111, 0b11111110,
    0b01111111, 0b11111110,
    0b00111111, 0b11111100,
    0b00011111, 0b11111000,
    0b00001111, 0b11110000,
    0b00000111, 0b11100000,
    0b00000011, 0b11000000,
    0b00000001, 0b10000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000};

// ========================
// MQTT Reconnect Function
// ========================
void reconnect()
{
  static unsigned long lastAttempt = 0;
  if (millis() - lastAttempt < 5000)
    return;
  lastAttempt = millis();

  if (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client"))
    {
      Serial.println("connected");
      client.publish(watch_topic, "ESP32 connected!");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.println(client.state());
    }
  }
}

// ========================
// MQTT Task (runs on Core 1)
// ========================
void mqttTask(void *parameter)
{
  for (;;)
  {
    if (!client.connected())
      reconnect();

    client.loop();

    static unsigned long lastMsg = 0;
    if (millis() - lastMsg > 5000)
    {
      lastMsg = millis();

      char msg[10];
      sprintf(msg, "%d", beatAvg);
      client.publish(watch_topic, msg);
      Serial.printf("MQTT publish: %s\n", msg);
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// ========================
// Heart Rate Sensor Update
// ========================
void update_HearateSensor()
{
  unsigned long irVal = hr_sensor.getIR();

  if (checkForBeat(irVal))
  {
    unsigned long now = millis();
    unsigned long time_per_beat = now - lastBeat;
    lastBeat = now;

    beat_per_min = 60.0 * 1000.0 / time_per_beat;

    rates[rate_idx++] = (byte)beat_per_min;
    rate_idx %= BUFF_SIZE;

    beatAvg = 0;
    for (byte i = 0; i < BUFF_SIZE; i++)
      beatAvg += rates[i];
    beatAvg /= BUFF_SIZE;
  }

  if (irVal < 50000)
  {
    for (byte i = 0; i < BUFF_SIZE; i++)
      rates[i] = 0;
    beatAvg = 0;
  }
}

// ========================
// OLED Display Update
// ========================
void display_screen()
{
  display.clearDisplay();
  display.drawBitmap(0, 8, heart, 16, 16, (togle ? WHITE : BLACK));
  display.setCursor(40, 8);
  display.setTextSize(2);
  display.setTextColor(WHITE, BLACK);
  display.printf("%d BPM", beatAvg);
  display.display();
}

// ========================
// Setup
// ========================
void setup()
{
  Serial.begin(115200);
  Serial.println("\n[ESP32] Initializing...");

  // -------- WiFi Connect --------
  WiFi.mode(WIFI_STA);

  // เริ่ม Wi-Fi manager แบบ non-blocking
  wm.setTimeout(30); // ตั้งเวลาสำหรับ config portal เป็นวินาที
  if (!wm.autoConnect("AutoConnectAP", "password"))
  {
    Serial.println("Failed to connect, continuing anyway...");
  }
  Serial.print("Connecting to WiFi");

  // -------- MQTT Setup --------
  client.setServer(mqtt_server, 1883);
  // reconnect();

  // -------- Create MQTT Task (on Core 0) --------
  xTaskCreatePinnedToCore(
      mqttTask,
      "MQTT Task",
      4096,
      NULL,
      1,
      &mqttTaskHandle,
      0);

  // -------- Heartrate Sensor Init --------
  if (!hr_sensor.begin(Wire, I2C_SPEED_FAST))
  {
    Serial.println("MAX30102 not found. Check wiring!");
    while (1)
      vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  hr_sensor.setup();
  hr_sensor.setPulseAmplitudeGreen(0);
  hr_sensor.setPulseAmplitudeRed(0x26);
  Serial.println("Heart rate sensor initialized.");

  // -------- OLED Display Init --------
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("SSD1306 not found. Check wiring!");
    while (1)
      vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  display.clearDisplay();
  display.display();

  Serial.println("System Ready ✅");
}

// ========================
// Main Loop (Core 0)
// ========================
void loop()
{
  update_HearateSensor();

  // // 🔹 ถ้า Wi-Fi ยังไม่เชื่อม ให้พยายามเชื่อมต่ออัตโนมัติ (non-blocking)
  // if (WiFi.status() != WL_CONNECTED)
  // {
  //   WiFi.mode(WIFI_STA);
  //   Serial.println("WiFi not connected, retrying...");
  //   wm.process(); // ให้ WiFiManager ทำงานเบื้องหลังโดยไม่บล็อก
  // }

  unsigned long now = millis();
  if (now - last_time >= period)
  {
    display_screen();
    last_time = now;
    togle = !togle;

    // 🔹 ถ้า Wi-Fi ยังไม่เชื่อม ให้พยายามเชื่อมต่ออัตโนมัติ (non-blocking)
    if (WiFi.status() != WL_CONNECTED)
    {
      WiFi.mode(WIFI_STA);
      Serial.println("WiFi not connected, retrying...");
      wm.process(); // ให้ WiFiManager ทำงานเบื้องหลังโดยไม่บล็อก
    }
  }

  
}
